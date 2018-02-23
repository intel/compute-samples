/*
 * Copyright(c) 2017 Intel Corporation
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

#include "median_filter/median_filter.hpp"

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/log/sources/record_ostream.hpp>

#include <boost/compute/memory_object.hpp>
#include <boost/compute/utility.hpp>

#include "timer/timer.hpp"

namespace compute_samples {
void MedianFilterApplication::run(std::vector<std::string> &command_line,
                                  src::logger &logger) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return;

  const compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();
  compute::context context(device);
  compute::command_queue queue(context, device);

  run_median_filter(args, context, queue, logger);
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
  options("kernel", po::value<std::string>(&args.kernel_path)
                        ->default_value("median_filter.cl"),
          "path to kernel");

  po::positional_options_description p;
  p.add("input-image", 1);
  p.add("output-image", 1);

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
  return args;
}

void MedianFilterApplication::run_median_filter(
    const MedianFilterApplication::Arguments &args, compute::context &context,
    compute::command_queue &queue, src::logger &logger) const {
  Timer timer_total(logger);
  Timer timer(logger);

  ImagePNG32Bit image(args.input_image_path);
  BOOST_LOG(logger) << "Input image path: " << args.input_image_path;
  BOOST_LOG(logger) << "Image size: " << image.width() << "x" << image.height()
                    << " pixels";
  BOOST_LOG(logger) << "Number of channels: " << image.number_of_channels();
  timer.print("Image read");

  compute::buffer input_buffer(context, image.size_in_bytes(),
                               compute::memory_object::read_only);
  compute::buffer output_buffer(context, image.size_in_bytes(),
                                compute::memory_object::write_only);

  const std::string kernel_name = "median_filter";
  compute::program program =
      compute::program::create_with_source_file(args.kernel_path, context);
  program.build();
  compute::kernel kernel = program.create_kernel(kernel_name);
  kernel.set_args(input_buffer, output_buffer);
  BOOST_LOG(logger) << "Kernel path: " << args.kernel_path;
  BOOST_LOG(logger) << "Kernel name: " << kernel_name;
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
  BOOST_LOG(logger) << "Output image path: " << args.output_image_path;
  timer.print("Report");

  timer_total.print("Total");
}

void MedianFilterApplication::write_image_to_buffer(
    const ImagePNG32Bit &image, compute::buffer &buffer,
    compute::command_queue &queue) const {
  uint32_t *mapped_buffer = static_cast<uint32_t *>(queue.enqueue_map_buffer(
      buffer, compute::command_queue::map_write, 0, image.size_in_bytes()));
  std::copy(image.raw_data(), image.raw_data() + image.size(), mapped_buffer);
  queue.enqueue_unmap_buffer(buffer, mapped_buffer);
}

void MedianFilterApplication::read_buffer_to_image(
    compute::buffer &buffer, ImagePNG32Bit &image,
    compute::command_queue &queue) const {
  uint32_t *mapped_buffer = static_cast<uint32_t *>(queue.enqueue_map_buffer(
      buffer, compute::command_queue::map_read, 0, image.size_in_bytes()));
  image.copy_raw_data(mapped_buffer);
  queue.enqueue_unmap_buffer(buffer, mapped_buffer);
}
} // namespace compute_samples
