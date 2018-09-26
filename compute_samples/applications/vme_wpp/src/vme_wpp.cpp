/*
 * Copyright(c) 2018 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "vme_wpp/vme_wpp.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/log/sources/record_ostream.hpp>

#include <boost/compute/core.hpp>
#include <boost/compute/image.hpp>
#include <boost/compute/utility.hpp>

#include <CL/cl_ext_intel.h>

#include "align_utils/align_utils.hpp"
#include "timer/timer.hpp"
#include "ocl_utils/ocl_utils.hpp"
namespace au = compute_samples::align_utils;

namespace compute_samples {

VmeWppApplication::Arguments VmeWppApplication::parse_command_line(
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

  if (vm.count("help")) {
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

Application::Status
VmeWppApplication::run_implementation(std::vector<std::string> &command_line,
                                      src::logger &logger) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();

  if (!device.supports_extension(
          "cl_intel_device_side_avc_motion_estimation")) {
    BOOST_LOG(logger)
        << "The selected device doesn't support device-side motion estimation.";
    return Status::SKIP;
  }

  if (!device.check_version(2, 0)) {
    BOOST_LOG(logger) << "The selected device doesn't support OpenCL 2.0.";
    return Status::SKIP;
  }

  BOOST_LOG(logger) << "Input yuv path: " << args.input_yuv_path;
  BOOST_LOG(logger) << "Frame size: " << args.width << "x" << args.height
                    << " pixels";

  Timer timer_total(logger);

  compute::context context(device);
  compute::command_queue queue(context, device);

  Timer timer(logger);
  compute::program program =
      build_program(context, "vme_wpp.cl", "-cl-std=CL2.0");
  timer.print("Program created");

  compute::kernel ds_kernel = program.create_kernel("downsample_3_tier");
  compute::kernel hme_n_kernel = program.create_kernel("tier_n_hme");
  compute::kernel wpp_kernel = program.create_kernel("vme_wpp");
  timer.print("Kernels created");

  Capture *capture = Capture::create_file_capture(
      args.input_yuv_path, args.width, args.height, args.frames);
  const int frame_count =
      (args.frames) ? args.frames : capture->get_num_frames();
  FrameWriter *writer = FrameWriter::create_frame_writer(
      args.width, args.height, frame_count, args.output_bmp);

  PlanarImage *planar_image =
      PlanarImage::create_planar_image(args.width, args.height);
  capture->get_sample(0, *planar_image);
  timer.print("Read YUV frame 0 from disk to CPU linear memory.");

  writer->append_frame(*planar_image);

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

  size_t origin[] = {0, 0, 0};
  size_t region[] = {static_cast<size_t>(args.width),
                     static_cast<size_t>(args.height), 1};
  queue.enqueue_write_image(src_image, origin, region, planar_image->get_y(),
                            planar_image->get_pitch_y());
  timer.print("Copied frame 0 to tiled memory.");

  ds_kernel.set_args(src_image, src_image_2x, src_image_4x, src_image_8x);
  queue.enqueue_nd_range_kernel(
      ds_kernel, 2, nullptr,
      compute::dim(au::align16(au::align_units(args.width, 4)),
                   au::align_units(args.height, 16))
          .data(),
      compute::dim(16, 1).data());
  timer.print("Enqueued downsample_3_tier kernel for frame 0");

  for (int k = 1; k < frame_count; k++) {
    BOOST_LOG(logger) << "Processing frame " << k << "...\n";
    run_vme_wpp(args, device, context, queue, ds_kernel, hme_n_kernel,
                wpp_kernel, *capture, *planar_image, src_image, ref_image,
                src_image_2x, ref_image_2x, src_image_4x, ref_image_4x,
                src_image_8x, ref_image_8x, k, logger);
    writer->append_frame(*planar_image);
  }

  BOOST_LOG(logger) << "Wrote " << frame_count << " frames with overlaid "
                    << "motion vectors to " << args.output_yuv_path << " .\n";
  writer->write_to_file(args.output_yuv_path.c_str());

  FrameWriter::release(writer);
  Capture::release(capture);
  PlanarImage::release_image(planar_image);

  timer_total.print("Total");
  return Status::OK;
}

void VmeWppApplication::run_vme_wpp(
    const VmeWppApplication::Arguments &args, compute::device &device,
    compute::context &context, compute::command_queue &queue,
    compute::kernel &ds_kernel, compute::kernel &hme_n_kernel,
    compute::kernel &wpp_kernel, Capture &capture, PlanarImage &planar_image,
    compute::image2d &src_image, compute::image2d &ref_image,
    compute::image2d &src_2x_image, compute::image2d &ref_2x_image,
    compute::image2d &src_4x_image, compute::image2d &ref_4x_image,
    compute::image2d &src_8x_image, compute::image2d &ref_8x_image,
    int frame_idx, src::logger &logger) const {
  Timer timer(logger);

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

  hme_n_kernel.set_arg(0, src_8x_image);
  hme_n_kernel.set_arg(1, ref_8x_image);
  hme_n_kernel.set_arg(2, sizeof(cl_mem), NULL);
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

  int mb_image_width = au::align_units(width, 16);
  int mb_image_height = au::align_units(height, 16);
  int mb_count = mb_image_width * mb_image_height;
  int mv_image_width = mb_image_width * 4;
  int mv_image_height = mb_image_height * 4;
  int mv_count = mv_image_width * mv_image_height;

  uint32_t num_eu = device.compute_units();
  uint32_t num_threads_per_eu = 7;
  uint32_t max_threads = num_eu * num_threads_per_eu;
  uint32_t width_mb_sz = au::align_units(width, 16);
  uint32_t num_blocks = (width_mb_sz < max_threads) ? width_mb_sz : max_threads;
  uint32_t simd_size = 16;

  au::PageAlignedVector<cl_short2> mvs(au::align64(mv_count));
  au::PageAlignedVector<cl_ushort> residuals(au::align64(mv_count));
  au::PageAlignedVector<cl_uchar2> shapes(au::align64(mb_count));
  au::PageAlignedVector<cl_int> scoreboard(au::align64(width_mb_sz), 0);

  compute::buffer scoreboard_buffer(
      context, au::align64(width_mb_sz * sizeof(uint32_t)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, scoreboard.data());
  compute::buffer mv_buffer(context, au::align64(mv_count * sizeof(cl_short2)),
                            CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
                            mvs.data());
  compute::buffer residual_buffer(
      context, au::align64(mv_count * sizeof(cl_ushort)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, residuals.data());
  compute::buffer shape_buffer(
      context, au::align64(mb_count * sizeof(cl_uchar2)),
      CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, shapes.data());
  timer.print("Created opencl mem objects for tier 0 wpp kernel");

  cl_uchar qp = static_cast<cl_uchar>(args.qp);
  cl_uchar sad_adjustment = CL_AVC_ME_SAD_ADJUST_MODE_NONE_INTEL;
  cl_uchar pixel_mode = CL_AVC_ME_SUBPIXEL_MODE_QPEL_INTEL;
  wpp_kernel.set_args(src_image, ref_image, pred_buffer, mv_buffer,
                      residual_buffer, shape_buffer, scoreboard_buffer, qp,
                      sad_adjustment, pixel_mode);
  size_t local_size = 16;
  size_t global_size = num_blocks * simd_size;
  queue.enqueue_nd_range_kernel(wpp_kernel, 1, nullptr, &global_size,
                                &local_size);
  timer.print("Enquequed tier 0 vme_wpp kernel");

  queue.finish();
  timer.print("Kernel finished.");

  planar_image.overlay_vectors(reinterpret_cast<motion_vector *>(mvs.data()),
                               reinterpret_cast<inter_shape *>(shapes.data()));
}
} // namespace compute_samples
