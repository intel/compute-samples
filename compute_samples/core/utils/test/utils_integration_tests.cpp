/*
 * Copyright(c) 2018 Intel Corporation
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
