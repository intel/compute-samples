/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"

#include <cstdint>
#include <fstream>
#include <istream>
#include <iterator>

class VmeInteropSystemTests : public testing::Test {
protected:
  virtual void TearDown() { std::remove(output_file_.c_str()); }

  const std::string input_file_ = "foreman_176x144.yuv";
  const std::string output_file_ = "output_foreman_176x144.yuv";
};
