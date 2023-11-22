/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ocl_utils/ocl_utils.hpp"
#include "gtest/gtest.h"

#include "utils/utils.hpp"
#include <fstream>

#include <boost/compute/utility/source.hpp>
#include "test_harness/test_harness.hpp"
#include "logging/logging.hpp"

namespace cs = compute_samples;
namespace compute = boost::compute;

class BuildProgram : public testing::Test {
protected:
  void SetUp() override {
    device = compute::system::default_device();
    context = compute::context(device);
  }

  void TearDown() override {
    if (std::remove(cl_file.c_str()) != 0) {
      LOG_DEBUG << "Deleting file " << cl_file.c_str() << " failed";
    }
  }

  const std::string cl_file = "kernel.cl";
  const std::string spv_file = "kernel.spv";
  compute::device device;
  compute::context context;
};

HWTEST_F(BuildProgram, ValidProgram) {
  const char source[] =
      BOOST_COMPUTE_STRINGIZE_SOURCE(kernel void my_kernel(){});
  cs::save_text_file(source, cl_file);
  EXPECT_NE(compute::program(), cs::build_program(context, cl_file));
}

HWTEST_F(BuildProgram, InvalidProgram) {
  const char source[] =
      BOOST_COMPUTE_STRINGIZE_SOURCE(kernel invalid_type my_kernel(){});
  cs::save_text_file(source, cl_file);
  EXPECT_THROW(cs::build_program(context, cl_file), compute::opencl_error);
}

HWTEST_F(BuildProgram, BuildOptions) {
  const char source[] =
      BOOST_COMPUTE_STRINGIZE_SOURCE(kernel MY_TYPE my_kernel(){});
  cs::save_text_file(source, cl_file);
  EXPECT_NE(compute::program(),
            cs::build_program(context, cl_file, "-DMY_TYPE=void"));
}

HWTEST_F(BuildProgram, ValidProgramSpirV) {
  std::vector<uint8_t> spriv_file_source = {
      0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0b, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00,
      0x0b, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4f, 0x70, 0x65, 0x6e,
      0x43, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x03, 0x00, 0x00, 0x00, 0x70, 0x8e, 0x01, 0x00};
  compute_samples::save_binary_file(spriv_file_source, spv_file);
  EXPECT_NE(compute::program(),
            cs::build_program_il(context, spv_file, "-cl-std=CL2.0"));
}

HWTEST_F(BuildProgram, InvalidProgramSpirV) {
  std::vector<uint8_t> spriv_file_source = {
      0x0A, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0b, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00,
      0x0b, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4f, 0x70, 0x65, 0x6e,
      0x43, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x03, 0x00, 0x00, 0x00, 0x70, 0x8e, 0x01, 0x00};
  compute_samples::save_binary_file(spriv_file_source, spv_file);
  EXPECT_THROW(cs::build_program_il(context, spv_file, "-cl-std=CL2.0"),
               compute::opencl_error);
}
