/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "usm_hello_world/usm_hello_world.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "test_harness/test_harness.hpp"
namespace cs = compute_samples;

HWTEST(UsmHelloWorldIntegrationTests, ProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(context, "usm_hello_world.cl"));
}

HWTEST(UsmHelloWorldIntegrationTests, CopyBufferWithHostUsm) {
  std::vector<cl_uint> input(1024);
  std::iota(input.begin(), input.end(), 0u);
  const std::vector<cl_uint> output =
      cs::copy_buffer(input, compute::usm_type::host);
  EXPECT_EQ(input, output);
}

HWTEST(UsmHelloWorldIntegrationTests, CopyBufferWithDeviceUsm) {
  std::vector<cl_uint> input(1024);
  std::iota(input.begin(), input.end(), 0u);
  const std::vector<cl_uint> output =
      cs::copy_buffer(input, compute::usm_type::device);
  EXPECT_EQ(input, output);
}

HWTEST(UsmHelloWorldIntegrationTests, CopyBufferWithSharedUsm) {
  std::vector<cl_uint> input(1024);
  std::iota(input.begin(), input.end(), 0u);
  const std::vector<cl_uint> output =
      cs::copy_buffer(input, compute::usm_type::shared);
  EXPECT_EQ(input, output);
}
