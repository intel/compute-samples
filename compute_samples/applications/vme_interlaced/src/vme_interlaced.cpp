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

#include "vme_interlaced/vme_interlaced.hpp"

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
#include <boost/log/sources/record_ostream.hpp>

#include <CL/cl_ext_intel.h>
#include <thread>

#include "ocl_utils/ocl_utils.hpp"

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

  if (vm.count("help")) {
    std::cout << desc;
    args.help = true;
    return args;
  }

  po::notify(vm);

  if (!args.sub_test.compare("native")) {
    args.native = 1;
  } else if (!args.sub_test.compare("split")) {
    args.native = 0;
  } else {
    throw std::invalid_argument("Invalid sub-test");
  }

  return args;
}

Application::Status VmeInterlacedApplication::run_implementation(
    std::vector<std::string> &command_line, src::logger &logger) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  const compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();

  if (!device.supports_extension(
          "cl_intel_device_side_avc_motion_estimation")) {
    BOOST_LOG(logger)
        << "The selected device doesn't support device-side motion estimation.";
    return Status::SKIP;
  }

  BOOST_LOG(logger) << "Input yuv path: " << args.input_yuv_path;
  BOOST_LOG(logger) << "Frame size: " << args.width << "x" << args.height
                    << " pixels";

  Timer timer_total(logger);

  compute::context context(device);
  compute::command_queue queue(context, device);

  Timer timer(logger);
  compute::program program = build_program(context, "vme_interlaced.cl");
  timer.print("Program created");

  compute::kernel kernel = program.create_kernel("vme_interlaced");
  timer.print("Kernel created");

  Capture *capture = Capture::create_file_capture(
      args.input_yuv_path, args.width, args.height, args.frames);
  const int frame_count =
      (args.frames) ? args.frames : capture->get_num_frames();
  const int field_height = args.height / 2;

  FrameWriter *top_writer = FrameWriter::create_frame_writer(
      args.width, field_height, frame_count, args.output_bmp);
  FrameWriter *bot_writer = FrameWriter::create_frame_writer(
      args.width, field_height, frame_count, args.output_bmp);

  PlanarImage *top_planar_image =
      PlanarImage::create_planar_image(args.width, field_height);
  PlanarImage *bot_planar_image =
      PlanarImage::create_planar_image(args.width, field_height);

  if (args.native) {
    compute::image_format format(CL_R, CL_UNORM_INT8);
    compute::image2d ref_image(context, args.width, args.height, format);
    compute::image2d src_image(context, args.width, args.height, format);

    PlanarImage *planar_image =
        PlanarImage::create_planar_image(args.width, args.height);
    capture->get_sample(0, *planar_image);
    timer.print("Read YUV frame 0 from disk to CPU linear memory.");

    size_t origin[] = {0, 0, 0};
    size_t region[] = {static_cast<size_t>(args.width),
                       static_cast<size_t>(args.height), 1};
    queue.enqueue_write_image(src_image, origin, region, planar_image->get_y(),
                              planar_image->get_pitch_y());
    timer.print("Copied interlaced frame 0 to tiled memory.");

    capture->get_sample(0, *top_planar_image, true, 0);
    capture->get_sample(0, *bot_planar_image, true, 1);
    timer.print("Read YUV field frames 0 from disk to CPU linear memory.");

    top_writer->append_frame(*top_planar_image);
    bot_writer->append_frame(*bot_planar_image);

    for (int k = 1; k < frame_count; k++) {
      BOOST_LOG(logger) << "Processing frame " << k << "...\n";
      run_vme_interlaced_native(args, context, queue, kernel, *capture,
                                *planar_image, *top_planar_image,
                                *bot_planar_image, src_image, ref_image, k,
                                logger);
      top_writer->append_frame(*top_planar_image);
      bot_writer->append_frame(*bot_planar_image);
    }

    PlanarImage::release_image(planar_image);
  } else {
    compute::image_format format(CL_R, CL_UNORM_INT8);
    compute::image2d ref_image(context, args.width, field_height, format);
    compute::image2d src_image(context, args.width, field_height, format);

    FrameWriter *field_writer[] = {top_writer, bot_writer};
    PlanarImage *field_planar_image[] = {top_planar_image, bot_planar_image};

    for (int j = 0; j < 2; j++) {
      BOOST_LOG(logger) << "Processing field polarity " << j;
      capture->get_sample(0, *field_planar_image[j], true, j);
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
        BOOST_LOG(logger) << "Processing field frame " << k << "...\n";
        run_vme_interlaced_split(args, context, queue, kernel, *capture,
                                 *field_planar_image[j], src_image, ref_image,
                                 j, k, logger);
        field_writer[j]->append_frame(*field_planar_image[j]);
      }
    }
  }

  BOOST_LOG(logger) << "Wrote " << frame_count << " top frames with overlaid "
                    << "motion vectors to " << args.output_top_yuv_path
                    << " .\n";
  top_writer->write_to_file(args.output_top_yuv_path.c_str());

  BOOST_LOG(logger) << "Wrote " << frame_count << " bot frames with overlaid "
                    << "motion vectors to " << args.output_bot_yuv_path
                    << " .\n";
  bot_writer->write_to_file(args.output_bot_yuv_path.c_str());

  PlanarImage::release_image(top_planar_image);
  PlanarImage::release_image(bot_planar_image);
  FrameWriter::release(top_writer);
  FrameWriter::release(bot_writer);
  Capture::release(capture);

  timer_total.print("Total");
  return Status::OK;
}

void VmeInterlacedApplication::run_vme_interlaced_native(
    const VmeInterlacedApplication::Arguments &args, compute::context &context,
    compute::command_queue &queue, compute::kernel &kernel, Capture &capture,
    PlanarImage &planar_image, PlanarImage &top_planar_image,
    PlanarImage &bot_planar_image, compute::image2d &src_image,
    compute::image2d &ref_image, int frame_idx, src::logger &logger) const {
  Timer timer(logger);

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
                     &capture, &top_planar_image, &bot_planar_image, frame_idx,
                     &logger);

  run_vme_interlaced(context, queue, kernel, src_image, ref_image, top_mvs,
                     top_shapes, residuals, predictors, width, mb_count,
                     mv_count, mb_image_height, 1, 0, timer, logger);
  timer.print("Completed VME for next top field lines");

  run_vme_interlaced(context, queue, kernel, src_image, ref_image, bot_mvs,
                     bot_shapes, residuals, predictors, width, mb_count,
                     mv_count, mb_image_height, 1, 1, timer, logger);
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
    compute::command_queue &queue, compute::kernel &kernel, Capture &capture,
    PlanarImage &field_planar_image, compute::image2d &src_image,
    compute::image2d &ref_image, int polarity, int frame_idx,
    src::logger &logger) const {
  Timer timer(logger);

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
                     mv_count, mb_image_height, 0, polarity, timer, logger);
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
    Timer &timer, src::logger &logger) const {
  BOOST_LOG(logger) << "Creating opencl mem objects...";
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
    Capture *capture, PlanarImage *top_planar_image,
    PlanarImage *bot_planar_image, int frame_idx, src::logger *logger) const {
  Timer timer(*logger);
  capture->get_sample(frame_idx, *top_planar_image, true, 0);
  timer.print("Read YUV next top frame from disk to CPU linear memory");
  capture->get_sample(frame_idx, *bot_planar_image, true, 1);
  timer.print("Read YUV next bot frame from disk to CPU linear memory");
}
} // namespace compute_samples
