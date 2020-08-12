/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_INTEL_DEVICE_HPP
#define BOOST_COMPUTE_INTEL_DEVICE_HPP

#include <boost/compute/device.hpp>

#include "ocl_entrypoints/cl_intel_unified_shared_memory.h"

namespace boost {
namespace compute {

class device_intel : public device {
  using device::device;

public:
  device_intel() = default;
  explicit device_intel(const device &other) : device(other) {}

  cl_unified_shared_memory_capabilities_intel host_mem_capabilities() const {
    return get_info<cl_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_HOST_MEM_CAPABILITIES_INTEL);
  }

  cl_unified_shared_memory_capabilities_intel device_mem_capabilities() const {
    return get_info<cl_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_DEVICE_MEM_CAPABILITIES_INTEL);
  }

  cl_unified_shared_memory_capabilities_intel
  single_device_shared_mem_capabilities() const {
    return get_info<cl_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_SINGLE_DEVICE_SHARED_MEM_CAPABILITIES_INTEL);
  }

  cl_unified_shared_memory_capabilities_intel
  cross_device_shared_mem_capabilities() const {
    return get_info<cl_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_CROSS_DEVICE_SHARED_MEM_CAPABILITIES_INTEL);
  }

  cl_unified_shared_memory_capabilities_intel
  shared_system_mem_capabilities() const {
    return get_info<cl_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_SHARED_SYSTEM_MEM_CAPABILITIES_INTEL);
  }
};

} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_INTEL_DEVICE_HPP
