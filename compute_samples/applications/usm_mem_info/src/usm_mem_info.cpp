/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "usm_mem_info/usm_mem_info.hpp"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/compute/intel/usm.hpp>
namespace compute = boost::compute;

#include "logging/logging.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "ocl_utils/unified_shared_memory.hpp"

namespace compute_samples {
Application::Status UsmMemInfoApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  const compute::device device = compute::system::default_device();
  LOG_INFO << "OpenCL device: " << device.name();

  if (!device.supports_extension("cl_intel_unified_shared_memory_preview")) {
    LOG_ERROR << "cl_intel_unified_shared_memory_preview extension is required";
    return Status::SKIP;
  }

  LOG_INFO << "Unified shared memory type: " << args.type;
  compute::context context(compute::system::default_context());

  cl_int *pointer = nullptr;
  if (args.type == compute::usm_type::host) {
    pointer = compute::host_mem_alloc<cl_int>(context, nullptr, 16, 0);
  } else if (args.type == compute::usm_type::device) {
    pointer =
        compute::device_mem_alloc<cl_int>(context, device, nullptr, 16, 0);
  } else if (args.type == compute::usm_type::shared) {
    pointer =
        compute::shared_mem_alloc<cl_int>(context, device, nullptr, 16, 0);
  }

  LOG_INFO << "Allocated pointer: " << pointer;

  cl_unified_shared_memory_type_intel type = 0;

  type = compute::get_mem_alloc_info<cl_unified_shared_memory_type_intel>(
      context, pointer, CL_MEM_ALLOC_TYPE_INTEL);
  LOG_INFO << "Type of base pointer: " << compute::usm_type(type);

  type = compute::get_mem_alloc_info<cl_unified_shared_memory_type_intel>(
      context, pointer + 4, CL_MEM_ALLOC_TYPE_INTEL);
  LOG_INFO << "Type of offset pointer: " << compute::usm_type(type);

  type = compute::get_mem_alloc_info<cl_unified_shared_memory_type_intel>(
      context, pointer + 64, CL_MEM_ALLOC_TYPE_INTEL);
  LOG_INFO << "Type of out of range pointer: " << compute::usm_type(type);

  const cl_int *base = compute::get_mem_alloc_info<cl_int *>(
      context, pointer + 4, CL_MEM_ALLOC_BASE_PTR_INTEL);
  LOG_INFO << "Base of offset pointer: " << base;

  const size_t size = compute::get_mem_alloc_info<size_t>(
      context, pointer + 4, CL_MEM_ALLOC_SIZE_INTEL);
  LOG_INFO << "Size of offset pointer: " << size;

  compute::mem_free(context, pointer);

  return Status::OK;
}

UsmMemInfoApplication::Arguments UsmMemInfoApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("type", po::value<compute::usm_type>(&args.type)->required(),
          "unified shared memory type: host, device or shared");

  po::positional_options_description p;
  p.add("type", 1);

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

} // namespace compute_samples
