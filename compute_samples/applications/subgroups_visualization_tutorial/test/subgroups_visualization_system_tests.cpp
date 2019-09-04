/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"

#include <fstream>

#include "image/image.hpp"
#include "subgroups_visualization/subgroups_visualization.hpp"
#include "test_harness/test_harness.hpp"

class SubgroupsVisualizationSystemTests : public testing::Test {
protected:
  virtual void TearDown() { std::remove(output_file_.c_str()); }

  const std::string solution_cl_file = "subgroups_visualization_solution.cl";
  const std::string output_file_ = "output.bmp";
  const std::string reference_file_ =
      "test_subgroups_visualization_reference.bmp";
};

HWTEST_F(SubgroupsVisualizationSystemTests, GeneratesOutputImage) {
  compute_samples::SubgroupsVisualizationApplication application;
  std::vector<std::string> command_line = {"-k", solution_cl_file};

  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

  compute_samples::ImageBMP8Bit output_image(output_file_);
  compute_samples::ImageBMP8Bit reference_image(reference_file_);

  EXPECT_GT(output_image.size(), 0);
  EXPECT_GT(reference_image.size(), 0);
  EXPECT_EQ(output_image, reference_image);
}
