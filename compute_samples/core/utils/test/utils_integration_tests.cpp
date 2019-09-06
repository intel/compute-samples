/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "utils/utils.hpp"
#include "gtest/gtest.h"
#include <cstdio>

TEST(LoadTextFile, ValidFile) {
  const std::string text = compute_samples::load_text_file("text_file.txt");
  const std::string reference = "abc";
  EXPECT_EQ(reference, text);
}

TEST(LoadTextFile, NotExistingFile) {
  const std::string text = compute_samples::load_text_file("invalid/path");
  const std::string reference = "";
  EXPECT_EQ(reference, text);
}

TEST(SaveTextFile, ValidFile) {
  const std::string text = "xyz";
  const std::string path = "output.txt";

  compute_samples::save_text_file(text, path);
  const std::string output = compute_samples::load_text_file(path);
  std::remove(path.c_str());

  EXPECT_EQ(text, output);
}

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
