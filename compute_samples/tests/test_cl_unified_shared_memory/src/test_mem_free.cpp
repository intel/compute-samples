/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"
#include "test_harness/test_harness.hpp"

namespace compute = boost::compute;

namespace {

HWTEST(clMemFreeINTEL, GivenHostMemoryThenNoExceptionIsThrown) {
  const compute::usm_type memory_type = compute::usm_type::host;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  auto *p = compute::host_mem_alloc<cl_int>(context, nullptr, 1, 0);
  EXPECT_NO_THROW(compute::mem_free(context, p));
}

HWTEST(clMemFreeINTEL, GivenDeviceMemoryThenNoExceptionIsThrown) {
  const compute::usm_type memory_type = compute::usm_type::device;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device = compute::system::default_device();
  auto *p = compute::device_mem_alloc<cl_int>(context, device, nullptr, 1, 0);
  EXPECT_NO_THROW(compute::mem_free(context, p));
}

HWTEST(clMemFreeINTEL, GivenSharedMemoryThenNoExceptionIsThrown) {
  const compute::usm_type memory_type = compute::usm_type::shared;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device = compute::system::default_device();
  auto *p = compute::shared_mem_alloc<cl_int>(context, device, nullptr, 1, 0);
  EXPECT_NO_THROW(compute::mem_free(context, p));
}

HWTEST(clMemFreeINTEL, GivenNullPointerThenNoExceptionIsThrown) {
  compute::context context(compute::system::default_context());
  cl_int *p = nullptr;
  EXPECT_NO_THROW(compute::mem_free(context, p));
}

} // namespace
