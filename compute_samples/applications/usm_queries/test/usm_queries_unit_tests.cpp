/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "usm_queries/usm_queries.hpp"
namespace cs = compute_samples;

TEST(ToStringTest, UnifiedSharedMemoryAccess) {
  const cl_device_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL;
  const std::string expected = "CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}

TEST(ToStringTest, UnifiedSharedMemoryAtomicAccess) {
  const cl_device_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL;
  const std::string expected = "CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}

TEST(ToStringTest, UnifiedSharedMemoryConcurrentAccess) {
  const cl_device_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ACCESS_INTEL;
  const std::string expected =
      "CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}

TEST(ToStringTest, UnifiedSharedMemoryConcurrentAtomicAccess) {
  const cl_device_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ATOMIC_ACCESS_INTEL;
  const std::string expected =
      "CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ATOMIC_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}

TEST(ToStringTest, UnifiedSharedMemoryAccessAndAtomicAccess) {
  const cl_device_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL |
      CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL;
  const std::string expected = "CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL | "
                               "CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}
