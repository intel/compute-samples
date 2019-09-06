/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_search/vme_search.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/compute/core.hpp>
#include <boost/compute/image.hpp>
#include <boost/compute/utility.hpp>

#include <CL/cl_ext_intel.h>

#include "align_utils/align_utils.hpp"
#include "timer/timer.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "logging/logging.hpp"
namespace au = compute_samples::align_utils;

namespace compute_samples {

VmeSearchApplication::Arguments VmeSearchApplication::parse_command_line(
    const std::vector<std::string> &command_line) {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("sub_test,s",
          po::value<std::string>(&args.sub_test)->default_value("basic_search"),
          "subtest to run - basic_search, cost_heuristics_search, "
          "larger_search");
  options("output-bmp,b",
          po::value<bool>(&args.output_bmp)
              ->default_value(false)
              ->implicit_value(true),
          "output to bmp images for each frame");
  options("input-yuv,i",
          po::value<std::string>(&args.input_yuv_path)
              ->default_value("goal_1280x720.yuv"),
          "path to input yuv file");
  options("output-yuv,o",
          po::value<std::string>(&args.output_yuv_path)
              ->default_value("output_goal_1280x720.yuv"),
          "path to output yuv with motion vectors");
  options("qp,q", po::value<int>(&args.qp)->default_value(49),
          "quantization parameter value to use for estimation heuristics"
          "(higher for faster motion frames)");
  options("width,w", po::value<int>(&args.width)->default_value(1280),
          "width of input yuv");
  options("height,h", po::value<int>(&args.height)->default_value(720),
          "height of input yuv");
  options("frames,f", po::value<int>(&args.frames)->default_value(0),
          "number of frame to use for motion estimation (0 represents entire "
          "yuv sequence)");

  po::positional_options_description p;
  p.add("input-yuv", 1);
  p.add("output-yuv", 1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(command_line).options(desc).positional(p).run(),
      vm);

  if (vm.count("help")) {
    std::cout << desc;
    args.help = true;
    return args;
  }

  po::notify(vm);

  if (args.qp > 51) {
    throw std::invalid_argument("Invalid argument for qp. Valid range (0-51).");
  }

  if (args.sub_test.compare("basic_search") &&
      args.sub_test.compare("cost_heuristics_search") &&
      args.sub_test.compare("larger_search")) {
    throw std::invalid_argument("Invalid sub-test");
  }

  return args;
}

Application::Status VmeSearchApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  const compute::device device = compute::system::default_device();
  LOG_INFO << "OpenCL device: " << device.name();

  if (!device.supports_extension(
          "cl_intel_device_side_avc_motion_estimation")) {
    LOG_ERROR
        << "The selected device doesn't support device-side motion estimation.";
    return Status::SKIP;
  }

  LOG_INFO << "Input yuv path: " << args.input_yuv_path;
  LOG_INFO << "Frame size: " << args.width << "x" << args.height << " pixels";

  Timer timer_total;

  compute::context context(device);
  compute::command_queue queue(context, device);

  Timer timer;

  std::string kernel_path = "vme_";
  kernel_path += args.sub_test;
  kernel_path += ".cl";

  LOG_INFO << "Kernel path: " << kernel_path;

  compute::program program = build_program(context, kernel_path);
  timer.print("Program created");

  std::string kernel_name = "vme_";
  kernel_name += args.sub_test;

  compute::kernel kernel = program.create_kernel(kernel_name);
  timer.print("Kernel created");

  YuvCapture capture(args.input_yuv_path, args.width, args.height, args.frames);
  const int frame_count =
      (args.frames) ? args.frames : capture.get_num_frames();
  YuvWriter writer(args.width, args.height, frame_count, args.output_bmp);

  PlanarImage planar_image(args.width, args.height);
  capture.get_sample(0, planar_image);
  timer.print("Read YUV frame 0 from disk to CPU linear memory.");

  writer.append_frame(planar_image);

  compute::image_format format(CL_R, CL_UNORM_INT8);
  compute::image2d ref_image(context, args.width, args.height, format);
  compute::image2d src_image(context, args.width, args.height, format);

  size_t origin[] = {0, 0, 0};
  size_t region[] = {static_cast<size_t>(args.width),
                     static_cast<size_t>(args.height), 1};
  queue.enqueue_write_image(src_image, origin, region, planar_image.get_y(),
                            planar_image.get_pitch_y());
  timer.print("Copied frame 0 to tiled memory.");

  for (int k = 1; k < frame_count; k++) {
    LOG_INFO << "Processing frame " << k << "...";
    run_vme_search(args, context, queue, kernel, capture, planar_image,
                   src_image, ref_image, k);
    writer.append_frame(planar_image);
  }

  LOG_INFO << "Wrote " << frame_count << " frames with overlaid "
           << "motion vectors to " << args.output_yuv_path << " .";
  writer.write_to_file(args.output_yuv_path.c_str());

  timer_total.print("Total");
  return Status::OK;
}

void VmeSearchApplication::run_vme_search(
    const VmeSearchApplication::Arguments &args, compute::context &context,
    compute::command_queue &queue, compute::kernel &kernel, YuvCapture &capture,
    PlanarImage &planar_image, compute::image2d &src_image,
    compute::image2d &ref_image, int frame_idx) const {
  Timer timer;

  int width = args.width;
  int height = args.height;

  int mb_image_width = au::align_units(width, 16);
  int mb_image_height = au::align_units(height, 16);
  int mv_image_width = mb_image_width * 4;
  int mv_image_height = mb_image_height * 4;
  int mv_count = mv_image_width * mv_image_height;
  int mb_count = mb_image_width * mb_image_height;

  LOG_INFO << "Creating opencl mem objects...";

  au::PageAlignedVector<cl_short2> mvs(au::align64(mv_count));
  au::PageAlignedVector<cl_short> residuals(au::align64(mv_count));
  au::PageAlignedVector<cl_uchar2> shapes(au::align64(mb_count));

  cl_short2 default_predictor = {0, 0};
  au::PageAlignedVector<cl_short2> predictors(au::align64(mb_count),
                                              default_predictor);

  compute::buffer mv_buffer(context, au::align64(mv_count * sizeof(cl_short2)),
                            CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
                            mvs.data());
  compute::buffer residual_buffer(
      context, au::align64(mv_count * sizeof(cl_ushort)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, residuals.data());
  compute::buffer shape_buffer(
      context, au::align64(mb_count * sizeof(cl_uchar2)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, shapes.data());
  compute::buffer pred_buffer(
      context, au::align64(mb_count * sizeof(cl_short2)),
      CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, predictors.data());

  timer.print("Created opencl mem objects.");

  std::swap(ref_image, src_image);

  capture.get_sample(frame_idx, planar_image);
  timer.print("Read next YUV frame from disk to CPU linear memory.");

  size_t origin[] = {0, 0, 0};
  size_t region[] = {static_cast<size_t>(width), static_cast<size_t>(height),
                     1};
  queue.enqueue_write_image(src_image, origin, region, planar_image.get_y(),
                            planar_image.get_pitch_y());
  timer.print("Copied frame to GPU tiled memory.");

  cl_uchar qp = static_cast<cl_uchar>(args.qp);
  cl_uchar sad_adjustment = CL_AVC_ME_SAD_ADJUST_MODE_NONE_INTEL;
  cl_uchar pixel_mode = CL_AVC_ME_SUBPIXEL_MODE_QPEL_INTEL;
  cl_int iterations = static_cast<cl_int>(mb_image_height);
  kernel.set_args(src_image, ref_image, pred_buffer, mv_buffer, residual_buffer,
                  shape_buffer, qp, sad_adjustment, pixel_mode, iterations);
  size_t local_size = 16;
  size_t global_size = static_cast<size_t>(au::align16(width));
  queue.enqueue_nd_range_kernel(kernel, 1, nullptr, &global_size, &local_size);
  timer.print("Kernel queued.");

  queue.finish();
  timer.print("Kernel finished.");

  planar_image.overlay_vectors(reinterpret_cast<motion_vector *>(mvs.data()),
                               reinterpret_cast<inter_shape *>(shapes.data()));
}
} // namespace compute_samples
