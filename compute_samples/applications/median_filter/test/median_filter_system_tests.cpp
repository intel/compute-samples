/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"

#include <fstream>

#include "image/image.hpp"
#include "median_filter/median_filter.hpp"
#include "test_harness/test_harness.hpp"

class MedianFilterSystemTests : public testing::Test {
protected:
  virtual void TearDown() { std::remove(output_file_.c_str()); }

  const std::string input_file_ = "test_input.png";
  const std::string output_file_ = "test_output.png";
  const std::string reference_file_ = "test_reference.png";
};

HWTEST_F(MedianFilterSystemTests, ReturnsReferenceImage) {
  compute_samples::MedianFilterApplication application;
  std::vector<std::string> command_line = {input_file_, output_file_};

  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

  compute_samples::ImagePNG32Bit output_image(output_file_);
  compute_samples::ImagePNG32Bit reference_image(reference_file_);
  EXPECT_EQ(output_image, reference_image);
}
