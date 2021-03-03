/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_interlaced/vme_interlaced.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/program_options.hpp>

#include <boost/compute/core.hpp>
#include <boost/compute/image.hpp>
#include <boost/compute/utility.hpp>

#include <CL/cl_ext.h>
#include <thread>

#include "ocl_utils/ocl_utils.hpp"
#include "logging/logging.hpp"

namespace au = compute_samples::align_utils;
namespace po = boost::program_options;
namespace compute = boost::compute;

namespace compute_samples {

VmeInterlacedApplication::Arguments
VmeInterlacedApplication::parse_command_line(
    const std::vector<std::string> &command_line) {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("sub_test,s",
          po::value<std::string>(&args.sub_test)->default_value("native"),
          "subtest to run - native, split");
  options("output-bmp,b",
          po::value<bool>(&args.output_bmp)
              ->default_value(false)
              ->implicit_value(true),
          "output to bmp images for each frame");
  options("input-yuv,i",
          po::value<std::string>(&args.input_yuv_path)
              ->default_value("football_interlaced_720x480.yuv"),
          "path to input yuv file");
  options("output-top-yuv,t",
          po::value<std::string>(&args.output_top_yuv_path)
              ->default_value("output_top_football_interlaced_720x480.yuv"),
          "path to output top field frame yuv with motion vectors");
  options("output-bot-yuv,b",
          po::value<std::string>(&args.output_bot_yuv_path)
              ->default_value("output_bot_football_interlaced_720x480.yuv"),
          "path to output bot field frame yuv with motion vectors");
  options("width,w", po::value<int>(&args.width)->default_value(720),
          "width of input yuv");
  options("height,h", po::value<int>(&args.height)->default_value(480),
          "height of input yuv");
  options("frames,f", po::value<int>(&args.frames)->default_value(0),
          "number of frame to use for motion estimation (0 represents entire "
          "yuv sequence)");

  po::positional_options_description p;
  p.add("input-yuv", 1);
  p.add("output-top-yuv", 1);
  p.add("output-bot-yuv", 1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(command_line).options(desc).positional(p).run(),
      vm);

  if (vm.count("help") != 0u) {
    std::cout << desc;
    args.help = true;
    return args;
  }

  po::notify(vm);

  if (args.sub_test == "native") {
    args.native = 1;
  } else if (args.sub_test == "split") {
    args.native = 0;
  } else {
    throw std::invalid_argument("Invalid sub-test");
  }

  return args;
}

Application::Status VmeInterlacedApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help) {
    return Status::SKIP;
  }

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
  compute::program program = build_program(context, "vme_interlaced.cl");
  timer.print("Program created");

  compute::kernel kernel = program.create_kernel("vme_interlaced");
  timer.print("Kernel created");

  YuvCapture capture(args.input_yuv_path, args.width, args.height, args.frames);
  const int frame_count =
      (args.frames) != 0 ? args.frames : capture.get_num_frames();
  const int field_height = args.height / 2;

  YuvWriter top_writer(args.width, field_height, frame_count, args.output_bmp);
  YuvWriter bot_writer(args.width, field_height, frame_count, args.output_bmp);

  PlanarImage top_planar_image(args.width, field_height);
  PlanarImage bot_planar_image(args.width, field_height);

  if (args.native != 0) {
    compute::image_format format(CL_R, CL_UNORM_INT8);
    compute::image2d ref_image(context, args.width, args.height, format);
    compute::image2d src_image(context, args.width, args.height, format);

    PlanarImage planar_image(args.width, args.height);
    capture.get_sample(0, planar_image);
    timer.print("Read YUV frame 0 from disk to CPU linear memory.");

    size_t origin[] = {0, 0, 0};
    size_t region[] = {static_cast<size_t>(args.width),
                       static_cast<size_t>(args.height), 1};
    queue.enqueue_write_image(src_image, origin, region, planar_image.get_y(),
                              planar_image.get_pitch_y());
    timer.print("Copied interlaced frame 0 to tiled memory.");

    capture.get_sample(0, top_planar_image, true, 0);
    capture.get_sample(0, bot_planar_image, true, 1);
    timer.print("Read YUV field frames 0 from disk to CPU linear memory.");

    top_writer.append_frame(top_planar_image);
    bot_writer.append_frame(bot_planar_image);

    for (int k = 1; k < frame_count; k++) {
      LOG_INFO << "Processing frame " << k << "...";
      run_vme_interlaced_native(args, context, queue, kernel, capture,
                                planar_image, top_planar_image,
                                bot_planar_image, src_image, ref_image, k);
      top_writer.append_frame(top_planar_image);
      bot_writer.append_frame(bot_planar_image);
    }
  } else {
    compute::image_format format(CL_R, CL_UNORM_INT8);
    compute::image2d ref_image(context, args.width, field_height, format);
    compute::image2d src_image(context, args.width, field_height, format);

    YuvWriter *field_writer[] = {&top_writer, &bot_writer};
    PlanarImage *field_planar_image[] = {&top_planar_image, &bot_planar_image};

    for (int j = 0; j < 2; j++) {
      LOG_INFO << "Processing field polarity " << j;
      capture.get_sample(0, *field_planar_image[j], true, j);
      timer.print("Read YUV field frame 0 from disk to CPU linear memory.");

      field_writer[j]->append_frame(*field_planar_image[j]);

      size_t origin[] = {0, 0, 0};
      size_t region[] = {static_cast<size_t>(args.width),
                         static_cast<size_t>(field_height), 1};
      queue.enqueue_write_image(src_image, origin, region,
                                field_planar_image[j]->get_y(),
                                field_planar_image[j]->get_pitch_y());
      timer.print("Copied field frame 0 to tiled memory.");

      for (int k = 1; k < frame_count; k++) {
        LOG_INFO << "Processing field frame " << k << "...";
        run_vme_interlaced_split(args, context, queue, kernel, capture,
                                 *field_planar_image[j], src_image, ref_image,
                                 j, k);
        field_writer[j]->append_frame(*field_planar_image[j]);
      }
    }
  }

  LOG_INFO << "Wrote " << frame_count << " top frames with overlaid "
           << "motion vectors to " << args.output_top_yuv_path << " .";
  top_writer.write_to_file(args.output_top_yuv_path.c_str());

  LOG_INFO << "Wrote " << frame_count << " bot frames with overlaid "
           << "motion vectors to " << args.output_bot_yuv_path << " .";
  bot_writer.write_to_file(args.output_bot_yuv_path.c_str());

  timer_total.print("Total");
  return Status::OK;
}

void VmeInterlacedApplication::run_vme_interlaced_native(
    const VmeInterlacedApplication::Arguments &args, compute::context &context,
    compute::command_queue &queue, compute::kernel &kernel, YuvCapture &capture,
    PlanarImage &planar_image, PlanarImage &top_planar_image,
    PlanarImage &bot_planar_image, compute::image2d &src_image,
    compute::image2d &ref_image, int frame_idx) const {
  Timer timer;

  int width = args.width;
  int height = args.height;
  int mb_image_width = au::align_units(width, 16);
  int mb_image_height = au::align_units(height, 16) / 2;
  int mv_image_width = mb_image_width * 4;
  int mv_image_height = mb_image_height * 4;
  int mv_count = mv_image_width * mv_image_height;
  int mb_count = mb_image_width * mb_image_height;

  capture.get_sample(frame_idx, planar_image);
  timer.print("Read next YUV frame from disk to CPU linear memory.");

  std::swap(ref_image, src_image);

  size_t origin[] = {0, 0, 0};
  size_t region[] = {static_cast<size_t>(width), static_cast<size_t>(height),
                     1};
  queue.enqueue_write_image(src_image, origin, region, planar_image.get_y(),
                            planar_image.get_pitch_y());
  timer.print("Copied next frame to GPU tiled memory.");

  au::PageAlignedVector<cl_short2> top_mvs(au::align64(mv_count));
  au::PageAlignedVector<cl_uchar2> top_shapes(au::align64(mb_count));
  au::PageAlignedVector<cl_short2> bot_mvs(au::align64(mv_count));
  au::PageAlignedVector<cl_uchar2> bot_shapes(au::align64(mb_count));
  au::PageAlignedVector<cl_ushort> residuals(au::align64(mv_count));
  cl_short2 default_predictor = {0, 0};
  au::PageAlignedVector<cl_short2> predictors(au::align64(mb_count),
                                              default_predictor);
  std::thread thread(&VmeInterlacedApplication::get_field_capture_samples, this,
                     std::ref(capture), std::ref(top_planar_image),
                     std::ref(bot_planar_image), frame_idx);

  run_vme_interlaced(context, queue, kernel, src_image, ref_image, top_mvs,
                     top_shapes, residuals, predictors, width, mb_count,
                     mv_count, mb_image_height, 1, 0, timer);
  timer.print("Completed VME for next top field lines");

  run_vme_interlaced(context, queue, kernel, src_image, ref_image, bot_mvs,
                     bot_shapes, residuals, predictors, width, mb_count,
                     mv_count, mb_image_height, 1, 1, timer);
  timer.print("Completed VME for next bottom field lines");

  thread.join();
  queue.finish();
  timer.print("Kernels finished.");

  top_planar_image.overlay_vectors(
      reinterpret_cast<motion_vector *>(top_mvs.data()),
      reinterpret_cast<inter_shape *>(top_shapes.data()));
  bot_planar_image.overlay_vectors(
      reinterpret_cast<motion_vector *>(bot_mvs.data()),
      reinterpret_cast<inter_shape *>(bot_shapes.data()));
  timer.print("Completed overlaying vectors for next frame");
}

void VmeInterlacedApplication::run_vme_interlaced_split(
    const VmeInterlacedApplication::Arguments &args, compute::context &context,
    compute::command_queue &queue, compute::kernel &kernel, YuvCapture &capture,
    PlanarImage &field_planar_image, compute::image2d &src_image,
    compute::image2d &ref_image, int polarity, int frame_idx) const {
  Timer timer;

  int width = args.width;
  int height = args.height / 2;
  int mb_image_width = au::align_units(width, 16);
  int mb_image_height = au::align_units(height, 16);
  int mv_image_width = mb_image_width * 4;
  int mv_image_height = mb_image_height * 4;
  int mv_count = mv_image_width * mv_image_height;
  int mb_count = mb_image_width * mb_image_height;

  capture.get_sample(frame_idx, field_planar_image, true, polarity);
  timer.print("Read next YUV frame from disk to CPU linear memory.");

  std::swap(ref_image, src_image);

  size_t origin[] = {0, 0, 0};
  size_t region[] = {static_cast<size_t>(width), static_cast<size_t>(height),
                     1};
  queue.enqueue_write_image(src_image, origin, region,
                            field_planar_image.get_y(),
                            field_planar_image.get_pitch_y());
  timer.print("Copied next field frame to GPU tiled memory.");

  au::PageAlignedVector<cl_short2> field_mvs(au::align64(mv_count));
  au::PageAlignedVector<cl_uchar2> field_shapes(au::align64(mb_count));
  au::PageAlignedVector<cl_ushort> residuals(au::align64(mv_count));
  cl_short2 default_predictor = {0, 0};
  au::PageAlignedVector<cl_short2> predictors(au::align64(mb_count),
                                              default_predictor);

  run_vme_interlaced(context, queue, kernel, src_image, ref_image, field_mvs,
                     field_shapes, residuals, predictors, width, mb_count,
                     mv_count, mb_image_height, 0, polarity, timer);
  queue.finish();
  timer.print("Kernel finished.");

  field_planar_image.overlay_vectors(
      reinterpret_cast<motion_vector *>(field_mvs.data()),
      reinterpret_cast<inter_shape *>(field_shapes.data()));
  timer.print("Completed overlaying vectors for next frame");
}

void VmeInterlacedApplication::run_vme_interlaced(
    compute::context &context, compute::command_queue &queue,
    compute::kernel &kernel, compute::image2d &src_image,
    compute::image2d &ref_image, au::PageAlignedVector<cl_short2> &mvs,
    au::PageAlignedVector<cl_uchar2> &shapes,
    au::PageAlignedVector<cl_ushort> &residuals,
    au::PageAlignedVector<cl_short2> &predictors, int width, int mb_count,
    int mv_count, uint32_t iterations, uint8_t interlaced, int polarity,
    Timer &timer) const {
  LOG_INFO << "Creating opencl mem objects...";
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

  kernel.set_args(src_image, ref_image, interlaced,
                  static_cast<uint8_t>(polarity), pred_buffer, mv_buffer,
                  residual_buffer, shape_buffer, iterations);
  size_t local_size = 16;
  size_t global_size = au::align16(width);
  queue.enqueue_nd_range_kernel(kernel, 1, nullptr, &global_size, &local_size);
  timer.print("Kernel queued.");
}

void VmeInterlacedApplication::get_field_capture_samples(
    YuvCapture &capture, PlanarImage &top_planar_image,
    PlanarImage &bot_planar_image, int frame_idx) const {
  Timer timer;
  capture.get_sample(frame_idx, top_planar_image, true, 0);
  timer.print("Read YUV next top frame from disk to CPU linear memory");
  capture.get_sample(frame_idx, bot_planar_image, true, 1);
  timer.print("Read YUV next bot frame from disk to CPU linear memory");
}
} // namespace compute_samples
