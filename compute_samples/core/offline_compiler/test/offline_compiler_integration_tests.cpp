/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <fstream>
#include "logging/logging.hpp"
#include "utils/utils.hpp"
#include "offline_compiler/offline_compiler.hpp"
#include "test_harness/test_harness.hpp"
#include "gtest/gtest.h"

namespace cs = compute_samples;

HWTEST(OfflineCompilerIntegrationTests, GenerateSPIRVFromFile) {
  const std::string source = "kernel void my_kernel(){}";
  const std::string cl_path = "kernel.cl";
  cs::save_text_file(source, cl_path);

  const std::string spv_path = cs::generate_spirv(cl_path);

  std::ifstream spv_file(spv_path.c_str());
  EXPECT_TRUE(spv_file.good());
  EXPECT_TRUE(spv_file.peek() != std::ifstream::traits_type::eof());

  if (std::remove(cl_path.c_str()) != 0) {
    LOG_DEBUG << "Deleting file " << cl_path.c_str() << " failed";
  }
  if (std::remove(spv_path.c_str()) != 0) {
    LOG_DEBUG << "Deleting file " << spv_path.c_str() << " failed";
  }
}

HWTEST(OfflineCompilerIntegrationTests, GenerateSPIRVFromFileWithBuildOptions) {
  const std::string source = "kernel void my_kernel(){int x = MY_VARIABLE;}";
  const std::string cl_path = "kernel.cl";
  cs::save_text_file(source, cl_path);
  const std::string spv_path = cs::generate_spirv(cl_path, "-DMY_VARIABLE=3");

  std::ifstream spv_file(spv_path.c_str());
  EXPECT_TRUE(spv_file.good());
  EXPECT_TRUE(spv_file.peek() != std::ifstream::traits_type::eof());

  if (std::remove(cl_path.c_str()) != 0) {
    LOG_DEBUG << "Deleting file " << cl_path.c_str() << " failed";
  }
  if (std::remove(spv_path.c_str()) != 0) {
    LOG_DEBUG << "Deleting file " << spv_path.c_str() << " failed";
  }
}

HWTEST(OfflineCompilerIntegrationTests, GenerateSPIRVFromFileInvalidPath) {
  const std::string cl_path = "not_existing.cl";
  EXPECT_THROW(cs::generate_spirv(cl_path), std::runtime_error);
}

HWTEST(OfflineCompilerIntegrationTests, GenerateSPIRVFromFileInvalidKernel) {
  const std::string source = "kernel void xyz";
  const std::string cl_path = "kernel.cl";
  cs::save_text_file(source, cl_path);
  EXPECT_THROW(cs::generate_spirv(cl_path), std::runtime_error);
  if (std::remove(cl_path.c_str()) != 0) {
    LOG_DEBUG << "Deleting file " << cl_path.c_str() << " failed";
  }
}

HWTEST(OfflineCompilerIntegrationTests, GenerateSPIRVFromSource) {
  const std::string source = "kernel void my_kernel(){}";
  const std::vector<uint8_t> spirv = cs::generate_spirv_from_source(source);
  EXPECT_FALSE(spirv.empty());
}

HWTEST(OfflineCompilerIntegrationTests,
       GenerateSPIRVFromSourceWithBuildOptions) {
  const std::string source = "kernel void my_kernel(){int x = MY_VARIABLE;}";
  const std::vector<uint8_t> spirv =
      cs::generate_spirv_from_source(source, "-DMY_VARIABLE=3");
  EXPECT_FALSE(spirv.empty());
}

HWTEST(OfflineCompilerIntegrationTests, GenerateSPIRVFromSourceInvalidKernel) {
  const std::string source = "kernel void xyz";
  EXPECT_THROW(cs::generate_spirv_from_source(source), std::runtime_error);
}
