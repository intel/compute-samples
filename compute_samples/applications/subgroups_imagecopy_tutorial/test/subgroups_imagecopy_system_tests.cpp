/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"

#include <fstream>

#include "image/image.hpp"
#include "subgroups_imagecopy/subgroups_imagecopy.hpp"
#include "test_harness/test_harness.hpp"

class SubgroupsImageCopySystemTests : public testing::Test {
protected:
  void TearDown() override { std::remove(output_file_.c_str()); }

  const std::string reference_file_ = "input.bmp";
  const std::string output_file_ = "output.bmp";
};

HWTEST_F(SubgroupsImageCopySystemTests, BasicCopy) {
  compute_samples::SubgroupsImageCopyApplication application;
  std::vector<std::string> command_line = {"-k",
                                           "subgroups_imagecopy_kernel.cl"};

  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

  compute_samples::ImageBMP8Bit output_image(output_file_);
  compute_samples::ImageBMP8Bit reference_image(reference_file_);

  EXPECT_GT(output_image.size(), 0);
  EXPECT_GT(reference_image.size(), 0);
  EXPECT_EQ(output_image, reference_image);
}

HWTEST_F(SubgroupsImageCopySystemTests, OptimizedCopy) {
  compute_samples::SubgroupsImageCopyApplication application;
  std::vector<std::string> command_line = {"-k",
                                           "subgroups_imagecopy_solution.cl"};

  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

  compute_samples::ImageBMP8Bit output_image(output_file_);
  compute_samples::ImageBMP8Bit reference_image(reference_file_);

  EXPECT_GT(output_image.size(), 0);
  EXPECT_GT(reference_image.size(), 0);
  EXPECT_EQ(output_image, reference_image);
}
