/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "usm_hello_world/usm_hello_world.hpp"
#include "utils/utils.hpp"

#include <iostream>
#include <numeric>

#include <boost/program_options.hpp>

#include <boost/compute/intel/command_queue.hpp>
#include <boost/compute/intel/device.hpp>
#include <boost/compute/intel/kernel.hpp>
#include "ocl_utils/ocl_utils.hpp"
#include "ocl_utils/unified_shared_memory.hpp"
#include "logging/logging.hpp"
#include "timer/timer.hpp"

namespace po = boost::program_options;
namespace compute = boost::compute;

namespace compute_samples {
Application::Status UsmHelloWorldApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help) {
    return Status::SKIP;
  }

  const compute::device_intel device(compute::system::default_device());
  LOG_INFO << "OpenCL device: " << device.name();

  if (!device.supports_extension("cl_intel_unified_shared_memory")) {
    LOG_ERROR << "cl_intel_unified_shared_memory extension is required";
    return Status::SKIP;
  }

  cl_device_unified_shared_memory_capabilities_intel capabilities = 0;
  if (args.type == compute::usm_type::host) {
    capabilities = device.host_mem_capabilities();
  } else if (args.type == compute::usm_type::device) {
    capabilities = device.device_mem_capabilities();
  } else if (args.type == compute::usm_type::shared) {
    capabilities = device.single_device_shared_mem_capabilities();
  }

  if ((capabilities & CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL) == 0u) {
    LOG_ERROR << "CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL capability is required";
    return Status::SKIP;
  }

  LOG_INFO << "Allocation size: " << args.size;
  LOG_INFO << "Unified shared memory type: " << args.type;

  std::vector<cl_uint> input(args.size);
  std::iota(input.begin(), input.end(), 0u);

  Timer timer_total;
  copy_buffer(input, args.type);
  timer_total.print("Total");

  return Status::OK;
}

UsmHelloWorldApplication::Arguments
UsmHelloWorldApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("type", po::value<compute::usm_type>(&args.type)->required(),
          "unified shared memory type: host, device or shared");
  options("size", po::value<size_t>(&args.size)->default_value(1024 * 1024),
          "number of elements to allocate");

  po::positional_options_description p;
  p.add("type", 1);

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

std::vector<cl_uint> copy_buffer(const std::vector<cl_uint> &input,
                                 const compute::usm_type type) {

  const compute::device device = compute::system::default_device();
  compute::context context(compute::system::default_context());
  compute::command_queue_intel queue(compute::system::default_queue());

  Timer timer;

  cl_uint *input_mem = nullptr;
  cl_uint *output_mem = nullptr;
  if (type == compute::usm_type::host) {
    input_mem =
        compute::host_mem_alloc<cl_uint>(context, nullptr, input.size(), 0);
    output_mem =
        compute::host_mem_alloc<cl_uint>(context, nullptr, input.size(), 0);
  } else if (type == compute::usm_type::device) {
    input_mem = compute::device_mem_alloc<cl_uint>(context, device, nullptr,
                                                   input.size(), 0);
    output_mem = compute::device_mem_alloc<cl_uint>(context, device, nullptr,
                                                    input.size(), 0);
  } else if (type == compute::usm_type::shared) {
    input_mem = compute::shared_mem_alloc<cl_uint>(context, device, nullptr,
                                                   input.size(), 0);
    output_mem = compute::shared_mem_alloc<cl_uint>(context, device, nullptr,
                                                    input.size(), 0);
  }
  timer.print("Unified shared memory allocated");

  if (type == compute::usm_type::host || type == compute::usm_type::shared) {
    std::copy(input.begin(), input.end(), input_mem);
  } else if (type == compute::usm_type::device) {
    queue.enqueue_memcpy(input_mem, input.data(), size_in_bytes(input));
  }
  timer.print("Input copied");

  compute::program program = build_program(context, "usm_hello_world.cl");
  compute::kernel_intel kernel(program.create_kernel("usm_hello_world_kernel"));
  kernel.set_arg_mem_ptr(0, input_mem);
  kernel.set_arg_mem_ptr(1, output_mem);
  timer.print("Kernel prepared");

  queue.enqueue_1d_range_kernel(kernel, 0, input.size(), 0);
  queue.finish();
  timer.print("Kernel finished");

  std::vector<cl_uint> output;
  if (type == compute::usm_type::host || type == compute::usm_type::shared) {
    output = std::vector<cl_uint>(output_mem, output_mem + input.size());
  } else if (type == compute::usm_type::device) {
    output = std::vector<cl_uint>(input.size());
    queue.enqueue_memcpy(output.data(), output_mem, size_in_bytes(output));
  }
  timer.print("Output copied");

  compute::mem_free(context, output_mem);
  compute::mem_free(context, input_mem);
  timer.print("Unified shared memory freed");

  return output;
}

} // namespace compute_samples
