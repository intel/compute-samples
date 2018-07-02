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

#include "gtest/gtest.h"

#include <fstream>

#include "image/image.hpp"
#include "median_filter/median_filter.hpp"

class MedianFilterSystemTests : public testing::Test {
protected:
  virtual void TearDown() { std::remove(output_file_.c_str()); }

  const std::string input_file_ = "test_input.png";
  const std::string output_file_ = "test_output.png";
  const std::string reference_file_ = "test_reference.png";
};

TEST_F(MedianFilterSystemTests, ReturnsReferenceImage) {
  compute_samples::MedianFilterApplication application;
  const char *argv[] = {"median_filter",
                        const_cast<char *>(input_file_.c_str()),
                        const_cast<char *>(output_file_.c_str()), nullptr};
  int argc = sizeof(argv) / sizeof(argv[0]) - 1;

  testing::internal::CaptureStdout();
  application.run(argc, argv);
  testing::internal::GetCapturedStdout();

  compute_samples::ImagePNG32Bit output_image(output_file_);
  compute_samples::ImagePNG32Bit reference_image(reference_file_);
  EXPECT_EQ(output_image, reference_image);
}
