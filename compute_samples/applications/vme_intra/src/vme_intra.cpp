/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_intra/vme_intra.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/program_options.hpp>

#include <boost/compute/image.hpp>
#include <boost/compute/utility.hpp>

#include <CL/cl_ext_intel.h>

#include "align_utils/align_utils.hpp"
#include "timer/timer.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "logging/logging.hpp"

namespace au = compute_samples::align_utils;
namespace po = boost::program_options;
namespace compute = boost::compute;

namespace compute_samples {

VmeIntraApplication::Arguments VmeIntraApplication::parse_command_line(
    const std::vector<std::string> &command_line) {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
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

  if (vm.count("help") != 0u) {
    std::cout << desc;
    args.help = true;
    return args;
  }

  po::notify(vm);

  if (args.qp > 51) {
    throw std::invalid_argument("Invalid argument for qp. Valid range (0-51).");
  }

  return args;
}

#define MV_PER_DIM (2)
#define DIM_TO_MB_SZ(X, Y) (au::align_units(au::align_units(X, Y), 16))
#define DIM_TO_MV_SZ(X, Y) (DIM_TO_MB_SZ(X, Y) * MV_PER_DIM)

void VmeIntraApplication::write_results_to_file(
    const cl_ulong *intra_modes, const cl_uchar *intra_shapes,
    const cl_ushort *intra_residuals, const cl_ushort *inter_best_residuals,
    uint32_t width, uint32_t height, uint32_t frame_idx) const {
  std::ofstream file;
  std::ios::openmode mode = (frame_idx > 0) ? std::ios::app : std::ios::trunc;
  mode |= std::ios::out;
  file.open("output_results.dat", mode);
  if (!file.good()) {
    throw std::runtime_error(
        "VmeIntraApplication::write_intra_modes_to_file: File open failed.");
  }

  uint32_t mb_count = au::align_units(width, 16) * au::align_units(height, 16);
  for (uint32_t i = 0; i < mb_count; i++) {
    uint32_t mb_x = i % au::align_units(width, 16);
    uint32_t mb_y = i / au::align_units(width, 16);

    file << "pic=" << frame_idx << " mb=(" << mb_x << "," << mb_y << "): ";
    file << uint16_t(intra_residuals[i]) << ": ";
    file << uint16_t(inter_best_residuals[i]) << ": ";

    if (intra_shapes[i] == CL_AVC_ME_INTRA_16x16_INTEL) {
      assert(uint32_t(intra_modes[i] & 0xF) < 4);
      file << uint32_t(intra_modes[i] & 0xF);
    } else if (intra_shapes[i] == CL_AVC_ME_INTRA_8x8_INTEL) {
      for (uint32_t j = 0; j < 12; j += 4) {
        assert(uint32_t((intra_modes[i] >> j * 4) & 0xF) < 9);
        file << uint32_t((intra_modes[i] >> j * 4) & 0xF) << ", ";
      }
      file << uint32_t((intra_modes[i] >> 48) & 0xF);
    } else if (intra_shapes[i] == CL_AVC_ME_INTRA_4x4_INTEL) {
      for (uint32_t j = 0; j < 15; j++) {
        assert(uint32_t((intra_modes[i] >> j * 4) & 0xF) < 9);
        file << uint32_t((intra_modes[i] >> j * 4) & 0xF) << ", ";
      }
      file << uint32_t((intra_modes[i] >> 60) & 0xF);
    } else {
      throw std::out_of_range("VmeIntraApplication::write_intra_modes_to_file: "
                              "Invalid intra shape value.");
    }

    file << std::endl;
  }
  file.close();
}

Application::Status VmeIntraApplication::run_implementation(
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
  compute::program program = build_program(context, "vme_intra.cl");
  timer.print("Program created");

  compute::kernel ds_kernel = program.create_kernel("downsample_3_tier");
  compute::kernel hme_n_kernel = program.create_kernel("tier_n_hme");
  compute::kernel intra_kernel = program.create_kernel("vme_intra");
  timer.print("Kernels created");

  YuvCapture capture(args.input_yuv_path, args.width, args.height, args.frames);
  const int frame_count =
      (args.frames) != 0 ? args.frames : capture.get_num_frames();
  YuvWriter writer(args.width, args.height, frame_count, args.output_bmp);

  PlanarImage planar_image(args.width, args.height);
  capture.get_sample(0, planar_image);
  timer.print("Read YUV frame 0 from disk to CPU linear memory.");

  compute::image_format format(CL_R, CL_UNORM_INT8);
  compute::image2d ref_image(context, args.width, args.height, format);
  compute::image2d src_image(context, args.width, args.height, format);
  compute::image2d src_image_8x(context, au::align_units(args.width, 8),
                                au::align_units(args.height, 8), format);
  compute::image2d ref_image_8x(context, au::align_units(args.width, 8),
                                au::align_units(args.height, 8), format);
  compute::image2d src_image_4x(context, au::align_units(args.width, 4),
                                au::align_units(args.height, 4), format);
  compute::image2d ref_image_4x(context, au::align_units(args.width, 4),
                                au::align_units(args.height, 4), format);
  compute::image2d src_image_2x(context, au::align_units(args.width, 2),
                                au::align_units(args.height, 2), format);
  compute::image2d ref_image_2x(context, au::align_units(args.width, 2),
                                au::align_units(args.height, 2), format);
  timer.print("Created opencl mem objects for downsample_3_tier kernel");

  for (int k = 0; k < frame_count; k++) {
    LOG_INFO << "Processing frame " << k << "...";
    run_vme_intra(args, context, queue, ds_kernel, hme_n_kernel, intra_kernel,
                  capture, planar_image, src_image, ref_image, src_image_2x,
                  ref_image_2x, src_image_4x, ref_image_4x, src_image_8x,
                  ref_image_8x, k);
    writer.append_frame(planar_image);
  }

  LOG_INFO << "Wrote " << frame_count << " frames with overlaid "
           << "motion vectors to " << args.output_yuv_path << " .";
  writer.write_to_file(args.output_yuv_path.c_str());

  timer_total.print("Total");
  return Status::OK;
}

void VmeIntraApplication::run_vme_intra(
    const VmeIntraApplication::Arguments &args, compute::context &context,
    compute::command_queue &queue, compute::kernel &ds_kernel,
    compute::kernel &hme_n_kernel, compute::kernel &intra_kernel,
    YuvCapture &capture, PlanarImage &planar_image, compute::image2d &src_image,
    compute::image2d &ref_image, compute::image2d &src_2x_image,
    compute::image2d &ref_2x_image, compute::image2d &src_4x_image,
    compute::image2d &ref_4x_image, compute::image2d &src_8x_image,
    compute::image2d &ref_8x_image, int frame_idx) const {
  Timer timer;

  int width = args.width;
  int height = args.height;

  std::swap(ref_image, src_image);
  std::swap(ref_2x_image, src_2x_image);
  std::swap(ref_4x_image, src_4x_image);
  std::swap(ref_8x_image, src_8x_image);

  capture.get_sample(frame_idx, planar_image);
  timer.print("Read next YUV frame from disk to CPU linear memory.");

  size_t origin[] = {0, 0, 0};
  size_t region[] = {static_cast<size_t>(width), static_cast<size_t>(height),
                     1};
  queue.enqueue_write_image(src_image, origin, region, planar_image.get_y(),
                            planar_image.get_pitch_y());
  timer.print("Copied next frame to GPU tiled memory.");

  ds_kernel.set_args(src_image, src_2x_image, src_4x_image, src_8x_image);
  queue.enqueue_nd_range_kernel(
      ds_kernel, 2, nullptr,
      compute::dim(au::align16(au::align_units(width, 4)),
                   au::align_units(height, 16))
          .data(),
      compute::dim(16, 1).data());
  timer.print("Enqueued downsample_3_tier kernel for next frame");

  uint32_t mv_8x_count = DIM_TO_MV_SZ(width, 8) * DIM_TO_MV_SZ(height, 8);
  au::PageAlignedVector<cl_short2> predictors_4x(au::align64(mv_8x_count));
  uint32_t mv_4x_count = DIM_TO_MV_SZ(width, 4) * DIM_TO_MV_SZ(height, 4);
  au::PageAlignedVector<cl_short2> predictors_2x(au::align64(mv_4x_count));
  uint32_t mv_2x_count = DIM_TO_MV_SZ(width, 2) * DIM_TO_MV_SZ(height, 2);
  au::PageAlignedVector<cl_short2> predictors(au::align64(mv_2x_count));

  compute::buffer pred_4x_buffer(
      context, au::align64(mv_8x_count * sizeof(cl_short2)),
      CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, predictors_4x.data());
  compute::buffer pred_2x_buffer(
      context, au::align64(mv_4x_count * sizeof(cl_short2)),
      CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, predictors_2x.data());
  compute::buffer pred_buffer(
      context, au::align64(mv_2x_count * sizeof(cl_short2)),
      CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, predictors.data());
  timer.print("Created opencl mem objects for tier_n_hme kernel");

  if (frame_idx > 0) {
    hme_n_kernel.set_arg(0, src_8x_image);
    hme_n_kernel.set_arg(1, ref_8x_image);
    hme_n_kernel.set_arg(2, sizeof(cl_mem), nullptr);
    hme_n_kernel.set_arg(3, pred_4x_buffer);
    queue.enqueue_nd_range_kernel(
        hme_n_kernel, 2, nullptr,
        compute::dim(au::align16(au::align_units(width, 8)),
                     DIM_TO_MB_SZ(height, 8))
            .data(),
        compute::dim(16, 1).data());
    timer.print("Enqueued tier 3 hme kernel for next frame");

    hme_n_kernel.set_args(src_4x_image, ref_4x_image, pred_4x_buffer,
                          pred_2x_buffer);
    queue.enqueue_nd_range_kernel(
        hme_n_kernel, 2, nullptr,
        compute::dim(au::align16(au::align_units(width, 4)),
                     DIM_TO_MB_SZ(height, 4))
            .data(),
        compute::dim(16, 1).data());
    timer.print("Enqueued tier 2 hme kernel for next frame");

    hme_n_kernel.set_args(src_2x_image, ref_2x_image, pred_2x_buffer,
                          pred_buffer);
    queue.enqueue_nd_range_kernel(
        hme_n_kernel, 2, nullptr,
        compute::dim(au::align16(au::align_units(width, 2)),
                     DIM_TO_MB_SZ(height, 2))
            .data(),
        compute::dim(16, 1).data());
    timer.print("Enqueued tier 1 hme kernel for next frame");
  } else {
    LOG_INFO << "Skipping hme for frame 0";
  }

  int mb_image_width = au::align_units(width, 16);
  int mb_image_height = au::align_units(height, 16);
  int mb_count = mb_image_width * mb_image_height;
  int mv_image_width = mb_image_width * 4;
  int mv_image_height = mb_image_height * 4;
  int mv_count = mv_image_width * mv_image_height;

  au::PageAlignedVector<cl_short2> mvs(au::align64(mv_count));
  au::PageAlignedVector<cl_uchar2> inter_shapes(au::align64(mb_count));
  au::PageAlignedVector<cl_ushort> inter_residuals(au::align64(mv_count),
                                                   0xFFFF);
  au::PageAlignedVector<cl_ushort> inter_best_residuals(au::align64(mb_count),
                                                        0xFFFF);
  au::PageAlignedVector<cl_uchar> intra_shapes(au::align64(mb_count), 0xFF);
  au::PageAlignedVector<cl_ushort> intra_residuals(au::align64(mb_count),
                                                   0xFFFF);
  au::PageAlignedVector<cl_ulong> intra_modes(au::align64(mb_count),
                                              0xFFFFFFFFFFFFFFFF);

  compute::buffer mv_buffer(context, au::align64(mv_count * sizeof(cl_short2)),
                            CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
                            mvs.data());
  compute::buffer inter_shape_buffer(
      context, au::align64(mb_count * sizeof(cl_uchar2)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, inter_shapes.data());
  compute::buffer inter_residual_buffer(
      context, au::align64(mv_count * sizeof(cl_ushort)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, inter_residuals.data());
  compute::buffer inter_best_residual_buffer(
      context, au::align64(mb_count * sizeof(cl_ushort)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, inter_best_residuals.data());
  compute::buffer intra_shape_buffer(
      context, au::align64(mb_count * sizeof(cl_uchar)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, intra_shapes.data());
  compute::buffer intra_residual_buffer(
      context, au::align64(mb_count * sizeof(cl_ushort)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, intra_residuals.data());
  compute::buffer intra_mode_buffer(
      context, au::align64(mb_count * sizeof(cl_ulong)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, intra_modes.data());
  timer.print("Created opencl mem objects for tier 0 intra kernel");

  auto qp = static_cast<cl_uchar>(args.qp);
  cl_uchar sad_adjustment = CL_AVC_ME_SAD_ADJUST_MODE_NONE_INTEL;
  cl_uchar pixel_mode = CL_AVC_ME_SUBPIXEL_MODE_QPEL_INTEL;
  auto iterations = static_cast<cl_int>(mb_image_height);
  auto intra_only = static_cast<cl_uchar>(frame_idx == 0);
  intra_kernel.set_args(src_image, ref_image, src_image, pred_buffer, mv_buffer,
                        inter_shape_buffer, inter_residual_buffer,
                        inter_best_residual_buffer, intra_shape_buffer,
                        intra_residual_buffer, intra_mode_buffer, qp,
                        sad_adjustment, pixel_mode, intra_only, iterations);
  size_t local_size = 16;
  size_t global_size = au::align16(width);
  compute::event event = queue.enqueue_nd_range_kernel(
      intra_kernel, 1, nullptr, &global_size, &local_size);
  timer.print("Enquequed tier 0 vme_intra kernel");

  event.wait();
  timer.print("Kernel finished.");

  planar_image.overlay_vectors(
      reinterpret_cast<motion_vector *>(mvs.data()),
      reinterpret_cast<inter_shape *>(inter_shapes.data()),
      reinterpret_cast<intra_shape *>(intra_shapes.data()),
      reinterpret_cast<residual *>(inter_best_residuals.data()),
      reinterpret_cast<residual *>(intra_residuals.data()));
  write_results_to_file(intra_modes.data(), intra_shapes.data(),
                        intra_residuals.data(), inter_best_residuals.data(),
                        width, height, frame_idx);
}
} // namespace compute_samples
