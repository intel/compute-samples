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

#include <cstdint>
#include <fstream>
#include <istream>
#include <iterator>

#include "vme_intra/vme_intra.hpp"
#include "test_harness/test_harness.hpp"

class VmeIntraSystemTests : public testing::Test {
protected:
  virtual void TearDown() { std::remove(output_file_.c_str()); }

  const std::string input_file_ = "foreman_176x144.yuv";
  const std::string output_file_ = "output_foreman_176x144.yuv";
};

HWTEST_F(VmeIntraSystemTests, ReturnsReferenceImage) {
  std::vector<std::string> command_line = {
      input_file_, output_file_, "--width", "176", "--height",
      "144",       "--qp",       "45",      "-f",  "50"};

  compute_samples::VmeIntraApplication application;
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

  std::ifstream out(output_file_, std::ios::binary);
  std::string reference_file = "intra_";
  reference_file += input_file_;
  std::ifstream ref(reference_file, std::ios::binary);
  EXPECT_TRUE(out.good() && ref.good());
  EXPECT_TRUE(!out.eof() && !ref.eof());

  std::istreambuf_iterator<char> out_iter(out);
  std::istreambuf_iterator<char> ref_iter(ref);
  std::istreambuf_iterator<char> eos_iter;
  while (ref_iter != eos_iter && out_iter != eos_iter) {
    EXPECT_EQ(*ref_iter++, *out_iter++);
  }

  EXPECT_EQ(out_iter, eos_iter);
  EXPECT_EQ(ref_iter, eos_iter);
}
