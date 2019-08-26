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

#include "gtest/gtest.h"
#include "usm_queries/usm_queries.hpp"
namespace cs = compute_samples;

TEST(ToStringTest, UnifiedSharedMemoryAccess) {
  const cl_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL;
  const std::string expected = "CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}

TEST(ToStringTest, UnifiedSharedMemoryAtomicAccess) {
  const cl_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL;
  const std::string expected = "CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}

TEST(ToStringTest, UnifiedSharedMemoryConcurrentAccess) {
  const cl_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ACCESS_INTEL;
  const std::string expected =
      "CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}

TEST(ToStringTest, UnifiedSharedMemoryConcurrentAtomicAccess) {
  const cl_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ATOMIC_ACCESS_INTEL;
  const std::string expected =
      "CL_UNIFIED_SHARED_MEMORY_CONCURRENT_ATOMIC_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}

TEST(ToStringTest, UnifiedSharedMemoryAccessAndAtomicAccess) {
  const cl_unified_shared_memory_capabilities_intel c =
      CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL |
      CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL;
  const std::string expected = "CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL | "
                               "CL_UNIFIED_SHARED_MEMORY_ATOMIC_ACCESS_INTEL";
  EXPECT_EQ(expected, cs::to_string(c));
}
