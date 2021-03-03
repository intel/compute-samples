/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_INTEL_DEVICE_HPP
#define BOOST_COMPUTE_INTEL_DEVICE_HPP

#include <boost/compute/device.hpp>

#include <CL/cl_ext.h>

namespace boost {
namespace compute {

class device_intel : public device {
  using device::device;

public:
  device_intel() = default;
  explicit device_intel(const device &other) : device(other) {}

  cl_device_unified_shared_memory_capabilities_intel
  host_mem_capabilities() const {
    return get_info<cl_device_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_HOST_MEM_CAPABILITIES_INTEL);
  }

  cl_device_unified_shared_memory_capabilities_intel
  device_mem_capabilities() const {
    return get_info<cl_device_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_DEVICE_MEM_CAPABILITIES_INTEL);
  }

  cl_device_unified_shared_memory_capabilities_intel
  single_device_shared_mem_capabilities() const {
    return get_info<cl_device_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_SINGLE_DEVICE_SHARED_MEM_CAPABILITIES_INTEL);
  }

  cl_device_unified_shared_memory_capabilities_intel
  cross_device_shared_mem_capabilities() const {
    return get_info<cl_device_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_CROSS_DEVICE_SHARED_MEM_CAPABILITIES_INTEL);
  }

  cl_device_unified_shared_memory_capabilities_intel
  shared_system_mem_capabilities() const {
    return get_info<cl_device_unified_shared_memory_capabilities_intel>(
        CL_DEVICE_SHARED_SYSTEM_MEM_CAPABILITIES_INTEL);
  }

  std::vector<size_t> sub_group_sizes() const {
    return get_info<std::vector<size_t>>(CL_DEVICE_SUB_GROUP_SIZES_INTEL);
  }

  cl_uint num_queue_families() { return get_number_of_families(*this); }

  std::vector<cl_queue_family_properties_intel>
  queue_family_properties() const {
    return get_queue_family_properties(*this);
  }

  static cl_uint get_number_of_families(const compute::device &device) {
    auto queue_family_properties =
        device.get_info<std::vector<cl_queue_family_properties_intel>>(
            CL_DEVICE_QUEUE_FAMILY_PROPERTIES_INTEL);
    return static_cast<cl_uint>(queue_family_properties.size());
  }

  static std::vector<cl_queue_family_properties_intel>
  get_queue_family_properties(const compute::device &device) {
    return device.get_info<std::vector<cl_queue_family_properties_intel>>(
        CL_DEVICE_QUEUE_FAMILY_PROPERTIES_INTEL);
  }
};

} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_INTEL_DEVICE_HPP
