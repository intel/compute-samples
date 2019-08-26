/*
 * Copyright(c) 2019 Intel Corporation
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

#include "usm_queries/usm_queries.hpp"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/compute/core.hpp>
#include <boost/compute/intel/device.hpp>
namespace compute = boost::compute;

#include "ocl_utils/ocl_utils.hpp"
#include "logging/logging.hpp"

namespace compute_samples {
Application::Status UsmQueriesApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help)
    return Status::SKIP;

  const compute::device_intel device(compute::system::default_device());
  LOG_INFO << "OpenCL device: " << device.name();

  LOG_INFO << "cl_intel_unified_shared_memory_preview extension supported: "
           << device.supports_extension(
                  "cl_intel_unified_shared_memory_preview");
  LOG_INFO << "CL_DEVICE_HOST_MEM_CAPABILITIES_INTEL: "
           << to_string(device.host_mem_capabilities());
  LOG_INFO << "CL_DEVICE_DEVICE_MEM_CAPABILITIES_INTEL: "
           << to_string(device.device_mem_capabilities());
  LOG_INFO << "CL_DEVICE_SINGLE_DEVICE_SHARED_MEM_CAPABILITIES_INTEL: "
           << to_string(device.single_device_shared_mem_capabilities());
  LOG_INFO << "CL_DEVICE_CROSS_DEVICE_SHARED_MEM_CAPABILITIES_INTEL: "
           << to_string(device.cross_device_shared_mem_capabilities());
  LOG_INFO << "CL_DEVICE_SHARED_SYSTEM_MEM_CAPABILITIES_INTEL: "
           << to_string(device.shared_system_mem_capabilities());

  return Status::OK;
}

UsmQueriesApplication::Arguments UsmQueriesApplication::parse_command_line(
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

std::string to_string(const cl_unified_shared_memory_capabilities_intel &x) {
  std::vector<std::string> v;
  if (x & CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL) {
    v.push_back("CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL");
  }
  if (x & CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL) {
    v.push_back("CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL");
  }
  if (x & CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ACCESS_INTEL) {
    v.push_back("CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ACCESS_INTEL");
  }
  if (x & CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ATOMIC_ACCESS_INTEL) {
    v.push_back("CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ATOMIC_ACCESS_INTEL");
  }

  std::ostringstream ss;
  for (size_t i = 0; i < v.size(); ++i) {
    ss << v[i];
    if (i < v.size() - 1) {
      ss << " | ";
    }
  }
  return ss.str();
}

} // namespace compute_samples
