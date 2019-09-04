/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "utils/utils.hpp"
#include "gtest/gtest.h"
#include <cstdio>

TEST(LoadBinaryFile, ValidFile) {
  const std::vector<uint8_t> bytes =
      compute_samples::load_binary_file("binary_file.bin");
  const std::vector<uint8_t> reference = {0x00, 0x11, 0x22, 0x33};
  EXPECT_EQ(reference, bytes);
}

TEST(LoadBinaryFile, NotExistingFile) {
  const std::vector<uint8_t> bytes =
      compute_samples::load_binary_file("invalid/path");
  const std::vector<uint8_t> reference = {};
  EXPECT_EQ(reference, bytes);
}

TEST(SaveBinaryFile, ValidFile) {
  const std::vector<uint8_t> bytes = {0x00, 0x11, 0x22, 0x33};
  const std::string path = "output.bin";

  compute_samples::save_binary_file(bytes, path);
  const std::vector<uint8_t> output = compute_samples::load_binary_file(path);
  std::remove(path.c_str());

  EXPECT_EQ(bytes, output);
}
