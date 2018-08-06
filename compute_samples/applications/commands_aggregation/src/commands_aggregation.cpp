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

#include "commands_aggregation/commands_aggregation.hpp"

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/log/sources/record_ostream.hpp>

#include <boost/compute/memory_object.hpp>
#include <boost/compute/utility.hpp>
#include <boost/compute/wait_list.hpp>

#include "timer/timer.hpp"

namespace compute_samples {
compute::kernel create_kernel(const compute::context &context,
                              src::logger &logger) {
  const std::string kernel_name = "commands_aggregation";
  compute::program program = compute::program::create_with_source_file(
      "commands_aggregation.cl", context);
  try {
    program.build("-cl-opt-disable");
  } catch (compute::opencl_error &) {
    BOOST_LOG(logger) << "OpenCL Program Build Error!";
    BOOST_LOG(logger) << "OpenCL Program Build Log is:\n"
                      << program.build_log();
  }
  return program.create_kernel(kernel_name);
}

std::vector<uint32_t> create_input_data(const int global_work_size) {
  const uint32_t number_of_kernels = 10;
  const int number_of_elements = number_of_kernels * global_work_size;
  return std::vector<uint32_t>(number_of_elements, 0);
}

compute::buffer create_buffer(const compute::context &context,
                              std::vector<uint32_t> &data) {
  const int data_size_in_bytes =
      static_cast<int>(data.size() * sizeof(uint32_t));
  return compute::buffer(
      context, data_size_in_bytes,
      compute::buffer::read_write | compute::buffer::use_host_ptr, data.data());
}

void CommandsAggregationApplication::run_implementation(
    std::vector<std::string> &command_line, src::logger &logger) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return;

  Timer timer(logger);
  if (args.in_order) {
    run_workloads_in_order(args.global_work_size, logger);
  } else {
    run_workloads_out_of_order(args.global_work_size, logger);
  }
  timer.print("Total");
}

CommandsAggregationApplication::Arguments
CommandsAggregationApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("in-order", "enable in-order execution in a single command queue");
  options("work-size",
          po::value<int>(&args.global_work_size)->default_value(512),
          "size of a global work group");

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

  if (vm.count("in-order")) {
    args.in_order = true;
  }

  po::notify(vm);
  return args;
}

std::vector<uint32_t>
CommandsAggregationApplication::run_workloads_out_of_order(
    const int global_work_size, src::logger &logger) const {
  BOOST_LOG(logger) << "Work size: " << global_work_size;

  const compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();
  compute::context context(device);

  std::vector<uint32_t> data = create_input_data(global_work_size);
  compute::buffer data_buffer = create_buffer(context, data);
  compute::kernel kernel = create_kernel(context, logger);

  compute::command_queue ioq1(context, device);
  compute::command_queue ioq2(context, device);

  // OOQ is needed to trigger aggregation logic as we get some events from
  // enqueue calls
  compute::command_queue ooq3(
      context, device,
      compute::command_queue::properties::enable_out_of_order_execution);

  uint32_t kernel_id = 0;

  // we measure clock timing on CPU as profiling will switch aggregation off
  Timer timer(logger);

  // cmd1
  kernel.set_args(data_buffer, kernel_id++);
  ioq1.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd2
  kernel.set_args(data_buffer, kernel_id++);
  compute::event ev1 =
      ioq2.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd3
  kernel.set_args(data_buffer, kernel_id++);
  ooq3.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd4
  kernel.set_args(data_buffer, kernel_id++);
  compute::event ev2 =
      ioq1.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd5
  kernel.set_args(data_buffer, kernel_id++);
  ioq2.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd6
  kernel.set_args(data_buffer, kernel_id++);
  compute::event ev3 =
      ioq1.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd7
  kernel.set_args(data_buffer, kernel_id++);
  compute::event ev4 =
      ioq2.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0, {ev2});

  // cmd8
  kernel.set_args(data_buffer, kernel_id++);
  ooq3.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0, {ev4});

  // cmd9
  kernel.set_args(data_buffer, kernel_id++);
  ooq3.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0, {ev1, ev3});

  // cmd10
  kernel.set_args(data_buffer, kernel_id++);
  ioq1.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  ioq1.flush();
  ioq2.flush();
  ooq3.flush();

  ioq1.finish();
  ioq2.finish();
  ooq3.finish();

  timer.print("Kernels execution");

  ioq1.enqueue_read_buffer(data_buffer, 0, data.size() * sizeof(uint32_t),
                           data.data());
  return data;
}

std::vector<uint32_t> CommandsAggregationApplication::run_workloads_in_order(
    const int global_work_size, src::logger &logger) const {
  BOOST_LOG(logger) << "Work size: " << global_work_size;

  const compute::device device = compute::system::default_device();
  BOOST_LOG(logger) << "OpenCL device: " << device.name();
  compute::context context(device);

  std::vector<uint32_t> data = create_input_data(global_work_size);
  compute::buffer data_buffer = create_buffer(context, data);
  compute::kernel kernel = create_kernel(context, logger);

  compute::command_queue ioq(context, device);

  uint32_t kernel_id = 0;
  Timer timer(logger);
  // cmd1
  kernel.set_args(data_buffer, kernel_id++);
  ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd2
  kernel.set_args(data_buffer, kernel_id++);
  compute::event ev1 =
      ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd3
  kernel.set_arg(1, kernel_id++);
  ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd4
  kernel.set_args(data_buffer, kernel_id++);
  compute::event ev2 =
      ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd5
  kernel.set_args(data_buffer, kernel_id++);
  ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd6
  kernel.set_args(data_buffer, kernel_id++);
  compute::event ev3 =
      ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  // cmd7
  kernel.set_args(data_buffer, kernel_id++);
  compute::event ev4 =
      ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0, {ev2});

  // cmd8
  kernel.set_args(data_buffer, kernel_id++);
  ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0, {ev4});

  // cmd9
  kernel.set_args(data_buffer, kernel_id++);
  ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0, {ev1, ev3});

  // cmd10
  kernel.set_args(data_buffer, kernel_id++);
  ioq.enqueue_1d_range_kernel(kernel, 0, global_work_size, 0);

  ioq.flush();
  ioq.finish();

  timer.print("Kernels execution");

  ioq.enqueue_read_buffer(data_buffer, 0, data.size() * sizeof(uint32_t),
                          data.data());
  return data;
}

} // namespace compute_samples
