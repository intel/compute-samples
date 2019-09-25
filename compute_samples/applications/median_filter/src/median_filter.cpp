/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "median_filter/median_filter.hpp"

#include <iostream>

#include <boost/program_options.hpp>

#include <boost/compute/memory_object.hpp>
#include <boost/compute/utility.hpp>

#include "timer/timer.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "logging/logging.hpp"

namespace po = boost::program_options;
namespace compute = boost::compute;

namespace compute_samples {
Application::Status MedianFilterApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help) {
    return Status::SKIP;
  }

  const compute::device device = compute::system::default_device();
  LOG_INFO << "OpenCL device: " << device.name();
  compute::context context(device);
  compute::command_queue queue(context, device);

  run_median_filter(args, context, queue);
  return Status::OK;
}

MedianFilterApplication::Arguments MedianFilterApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("input-image",
          po::value<std::string>(&args.input_image_path)->required(),
          "path to input image");
  options("output-image",
          po::value<std::string>(&args.output_image_path)->required(),
          "path to output image");
  options("kernel",
          po::value<std::string>(&args.kernel_path)
              ->default_value("median_filter.cl"),
          "path to kernel");

  po::positional_options_description p;
  p.add("input-image", 1);
  p.add("output-image", 1);

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
  return args;
}

void MedianFilterApplication::run_median_filter(
    const MedianFilterApplication::Arguments &args, compute::context &context,
    compute::command_queue &queue) const {
  Timer timer_total;
  Timer timer;

  ImagePNG32Bit image(args.input_image_path);
  LOG_INFO << "Input image path: " << args.input_image_path;
  LOG_INFO << "Image size: " << image.width() << "x" << image.height()
           << " pixels";
  LOG_INFO << "Number of channels: " << image.number_of_channels();
  timer.print("Image read");

  compute::buffer input_buffer(context, size_in_bytes(image),
                               compute::memory_object::read_only);
  compute::buffer output_buffer(context, size_in_bytes(image),
                                compute::memory_object::write_only);

  compute::program program = build_program(context, args.kernel_path);
  compute::kernel kernel = program.create_kernel("median_filter");
  kernel.set_args(input_buffer, output_buffer);
  LOG_INFO << "Kernel path: " << args.kernel_path;
  timer.print("Kernel created");

  write_image_to_buffer(image, input_buffer, queue);
  timer.print("Input queued");

  queue.enqueue_nd_range_kernel(
      kernel, 2, nullptr, compute::dim(image.width(), image.height()).data(),
      nullptr);
  timer.print("Kernel queued");

  queue.finish();
  timer.print("Kernel finished");

  read_buffer_to_image(output_buffer, image, queue);
  timer.print("Output ready");

  image.write(args.output_image_path);
  LOG_INFO << "Output image path: " << args.output_image_path;
  timer.print("Report");

  timer_total.print("Total");
}

void MedianFilterApplication::write_image_to_buffer(
    const ImagePNG32Bit &image, compute::buffer &buffer,
    compute::command_queue &queue) const {
  uint32_t *mapped_buffer = static_cast<uint32_t *>(queue.enqueue_map_buffer(
      buffer, compute::command_queue::map_write, 0, size_in_bytes(image)));
  std::copy(image.raw_data(), image.raw_data() + image.size(), mapped_buffer);
  queue.enqueue_unmap_buffer(buffer, mapped_buffer);
}

void MedianFilterApplication::read_buffer_to_image(
    compute::buffer &buffer, ImagePNG32Bit &image,
    compute::command_queue &queue) const {
  uint32_t *mapped_buffer = static_cast<uint32_t *>(queue.enqueue_map_buffer(
      buffer, compute::command_queue::map_read, 0, size_in_bytes(image)));
  image.copy_raw_data(mapped_buffer);
  queue.enqueue_unmap_buffer(buffer, mapped_buffer);
}
} // namespace compute_samples
