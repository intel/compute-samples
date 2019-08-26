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
#include "usm_hello_world/usm_hello_world.hpp"
#include "ocl_utils/ocl_utils.hpp"
namespace cs = compute_samples;

TEST(UsmHelloWorldIntegrationTests, ProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(context, "usm_hello_world.cl"));
}

TEST(UsmHelloWorldIntegrationTests, CopyBufferWithHostUsm) {
  std::vector<cl_uint> input(1024);
  std::iota(input.begin(), input.end(), 0u);
  const std::vector<cl_uint> output =
      cs::copy_buffer(input, compute::usm_type::host);
  EXPECT_EQ(input, output);
}

TEST(UsmHelloWorldIntegrationTests, CopyBufferWithDeviceUsm) {
  std::vector<cl_uint> input(1024);
  std::iota(input.begin(), input.end(), 0u);
  const std::vector<cl_uint> output =
      cs::copy_buffer(input, compute::usm_type::device);
  EXPECT_EQ(input, output);
}

TEST(UsmHelloWorldIntegrationTests, CopyBufferWithSharedUsm) {
  std::vector<cl_uint> input(1024);
  std::iota(input.begin(), input.end(), 0u);
  const std::vector<cl_uint> output =
      cs::copy_buffer(input, compute::usm_type::shared);
  EXPECT_EQ(input, output);
}
