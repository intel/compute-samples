/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"
#include "test_harness/test_harness.hpp"

#include "boost/compute/intel/device.hpp"

namespace compute = boost::compute;

namespace {

HWTEST(clGetDeviceInfo, GivenHostMemoryParameterThenNoExceptionIsThrown) {
  compute::device_intel device(compute::system::default_device());
  EXPECT_NO_THROW(device.host_mem_capabilities());
}

HWTEST(clGetDeviceInfo,
       GivenDeviceMemoryParameterThenRequiredCapabilitiesAreReturned) {
  compute::device_intel device(compute::system::default_device());
  const cl_unified_shared_memory_capabilities_intel capabilities =
      device.device_mem_capabilities();
  EXPECT_TRUE(capabilities & CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL);
}

HWTEST(clGetDeviceInfo,
       GivenSingleDeviceSharedMemoryParameterThenNoExceptionIsThrown) {
  compute::device_intel device(compute::system::default_device());
  EXPECT_NO_THROW(device.single_device_shared_mem_capabilities());
}

HWTEST(clGetDeviceInfo,
       GivenCrossDeviceSharedMemoryParameterThenNoExceptionIsThrown) {
  compute::device_intel device(compute::system::default_device());
  EXPECT_NO_THROW(device.cross_device_shared_mem_capabilities());
}

HWTEST(clGetDeviceInfo,
       GivenSharedSystemMemoryParameterThenNoExceptionIsThrown) {
  compute::device_intel device(compute::system::default_device());
  EXPECT_NO_THROW(device.shared_system_mem_capabilities());
}

HWTEST(clGetDeviceInfo, ExtensionIsSupported) {
  compute::device device = compute::system::default_device();
  EXPECT_TRUE(
      device.supports_extension("cl_intel_unified_shared_memory_preview"));
}

} // namespace
