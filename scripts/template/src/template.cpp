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

#include "template/template.hpp"

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/log/sources/record_ostream.hpp>

#include <boost/compute/memory_object.hpp>
#include <boost/compute/utility.hpp>

namespace compute_samples {
Application::Status
TemplateApplication::run_implementation(std::vector<std::string> &command_line,
                                        src::logger &logger) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  const compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();
  compute::context context(device);
  compute::command_queue queue(context, device);

  std::vector<cl_char> input = {72, 101, 108, 108, 111, 32,
                                87, 111, 114, 108, 100, 33};
  std::vector<cl_char> output(input.size(), 0);

  compute::buffer input_buffer(
      context, input.size() * sizeof(decltype(input)::value_type),
      compute::memory_object::read_only | compute::memory_object::use_host_ptr,
      input.data());
  compute::buffer output_buffer(
      context, output.size() * sizeof(decltype(output)::value_type),
      compute::memory_object::write_only);

  compute::kernel kernel = create_kernel(context, logger);
  kernel.set_args(input_buffer, output_buffer);

  queue.enqueue_1d_range_kernel(kernel, 0, input.size(), 0);

  queue.enqueue_read_buffer(output_buffer, 0, output.size(), output.data());
  BOOST_LOG(logger) << "Output: " << std::string(output.begin(), output.end());

  return Status::OK;
}

TemplateApplication::Arguments TemplateApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");

  po::positional_options_description p;

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

compute::kernel create_kernel(const compute::context &context,
                              src::logger &logger) {
  const std::string kernel_name = "template_kernel";
  compute::program program =
      compute::program::create_with_source_file("template.cl", context);
  try {
    program.build();
  } catch (compute::opencl_error &) {
    BOOST_LOG(logger) << "OpenCL Program Build Error!";
    BOOST_LOG(logger) << "OpenCL Program Build Log is:\n"
                      << program.build_log();
    throw;
  }
  return program.create_kernel(kernel_name);
}

std::string ascii_to_string(const std::vector<cl_char> &output) {
  return std::string(output.begin(), output.end());
}

} // namespace compute_samples
