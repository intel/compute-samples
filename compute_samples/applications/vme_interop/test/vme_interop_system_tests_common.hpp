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

#include "logging/logging.hpp"

class VmeInteropSystemTests : public testing::Test {
protected:
  virtual void TearDown() {
    if (std::remove(output_file_.c_str()) != 0) {
      LOG_DEBUG << "Deleting file " << output_file_.c_str() << " failed";
    }
  }

  const std::string input_file_ = "foreman_176x144.yuv";
  const std::string output_file_ = "output_foreman_176x144.yuv";
};
