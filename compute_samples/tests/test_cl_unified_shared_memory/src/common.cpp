/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"

#include "boost/compute/intel/device.hpp"

namespace compute = boost::compute;

cl_device_unified_shared_memory_capabilities_intel
get_capabilities(const compute::usm_type t) {
  const compute::device_intel device(compute::system::default_device());

  cl_device_unified_shared_memory_capabilities_intel capabilities = 0;
  if (t == compute::usm_type::host) {
    capabilities = device.host_mem_capabilities();
  } else if (t == compute::usm_type::device) {
    capabilities = device.device_mem_capabilities();
  } else if (t == compute::usm_type::shared) {
    capabilities = device.single_device_shared_mem_capabilities();
  } else {
    capabilities = device.shared_system_mem_capabilities();
  }

  return capabilities;
}

bool is_memory_access_supported(const compute::usm_type t) {
  return (get_capabilities(t) & CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL) != 0u;
}

bool is_atomic_access_supported(const compute::usm_type t) {
  return (get_capabilities(t) & CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL) !=
         0u;
}

std::string to_string(const CopyKernelCombination &x) {
  std::stringstream ss;
  ss << "{";
  ss << "allocation_count: " << x.allocation_count << ", ";
  ss << "region_count: " << x.region_count << ", ";
  ss << "source_offset: " << x.source_offset << ", ";
  ss << "destination_offset: " << x.destination_offset;
  ss << "}";
  return ss.str();
}

std::ostream &operator<<(std::ostream &os, const CopyKernelCombination &x) {
  os << to_string(x);
  return os;
}

std::string copy_kernel_test_name_suffix(
    testing::TestParamInfo<
        std::tuple<CopyKernelCombination, compute::usm_type, compute::usm_type>>
        param_info) {
  const CopyKernelCombination c = std::get<0>(param_info.param);
  const compute::usm_type source = std::get<1>(param_info.param);
  const compute::usm_type destination = std::get<2>(param_info.param);

  std::stringstream ss;
  ss << "allocation_count_" << c.allocation_count << "_";
  ss << "region_count_" << c.region_count << "_";
  ss << "source_offset_" << c.source_offset << "_";
  ss << "destination_offset_" << c.destination_offset << "_";
  ss << "source_" << source << "_";
  ss << "destination_" << destination;
  return ss.str();
}
