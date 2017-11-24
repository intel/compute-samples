/*
 * Copyright(c) 2017 Intel Corporation
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
#include "gtest/gtest.h"

#include <fstream>

#include "framework/framework.hpp"
#include "image/image.hpp"
#include "subgroups_visualization/subgroups_visualization.hpp"

class SubgroupsVisualizationSystemTests : public testing::Test {
protected:
  virtual void TearDown() { std::remove(output_file_.c_str()); }

  const std::string solution_cl_file = "subgroups_visualization_solution.cl";
  const std::string output_file_ = "output.bmp";
  const std::string reference_file_ =
      "test_subgroups_visualization_reference.bmp";
};

TEST_F(SubgroupsVisualizationSystemTests, GeneratesOutputImage) {
  compute_samples::Framework framework;
  const char *argv[] = {"compute_samples", "subgroups_visualization", "-k",
                        const_cast<char *>(solution_cl_file.c_str()), nullptr};
  int argc = sizeof(argv) / sizeof(argv[0]) - 1;

  testing::internal::CaptureStdout();
  framework.run(argc, argv);
  testing::internal::GetCapturedStdout();

  compute_samples::ImageBMP8Bit output_image(output_file_);
  compute_samples::ImageBMP8Bit reference_image(reference_file_);

  EXPECT_GT(output_image.size(), 0);
  EXPECT_GT(reference_image.size(), 0);
  EXPECT_EQ(output_image, reference_image);
}
