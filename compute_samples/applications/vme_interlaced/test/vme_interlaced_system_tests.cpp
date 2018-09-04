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

#include "vme_interlaced/vme_interlaced.hpp"

class VmeInterlacedSystemTests : public testing::Test {
protected:
  virtual void TearDown() {
    std::remove(output_top_file_.c_str());
    std::remove(output_bot_file_.c_str());
    std::remove(reference_top_file_.c_str());
    std::remove(reference_bot_file_.c_str());
  }

  const std::string input_file_ = "football_interlaced_720x480.yuv";
  const std::string output_top_file_ =
      "output_top_football_interlaced_720x480.yuv";
  const std::string output_bot_file_ =
      "output_bot_football_interlaced_720x480.yuv";
  const std::string reference_top_file_ = "top_football_interlaced_720x480.yuv";
  const std::string reference_bot_file_ = "bot_football_interlaced_720x480.yuv";
};

TEST_F(VmeInterlacedSystemTests, ReturnsReferenceImage) {
  const char *argv[] = {"vme_interlaced",
                        const_cast<char *>(input_file_.c_str()),
                        const_cast<char *>(output_top_file_.c_str()),
                        const_cast<char *>(output_bot_file_.c_str()),
                        "--width",
                        "720",
                        "--height",
                        "480",
                        "-s",
                        "native",
                        "-f",
                        "30",
                        nullptr};
  int argc = sizeof(argv) / sizeof(argv[0]) - 1;

  compute_samples::VmeInterlacedApplication application;

  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(argc, argv));

  static const int reference_top_file_pos = 2;
  static const int reference_bot_file_pos = reference_top_file_pos + 1;
  static const int sub_test_pos = reference_top_file_pos + 7;

  argv[reference_top_file_pos] = reference_top_file_.c_str();
  argv[reference_bot_file_pos] = reference_bot_file_.c_str();
  argv[sub_test_pos] = "split";

  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(argc, argv));

  std::ifstream out_top(output_top_file_, std::ios::binary);
  std::ifstream ref_top(reference_top_file_, std::ios::binary);
  EXPECT_TRUE(out_top.good() && ref_top.good());
  EXPECT_TRUE(!out_top.eof() && !ref_top.eof());

  std::istreambuf_iterator<char> out_top_iter(out_top);
  std::istreambuf_iterator<char> ref_top_iter(ref_top);
  std::istreambuf_iterator<char> eos_top_iter;
  while (ref_top_iter != eos_top_iter && out_top_iter != eos_top_iter) {
    EXPECT_EQ(*ref_top_iter++, *out_top_iter++);
  }

  std::ifstream out_bot(output_bot_file_, std::ios::binary);
  std::ifstream ref_bot(reference_bot_file_, std::ios::binary);
  EXPECT_TRUE(out_bot.good() && ref_bot.good());
  EXPECT_TRUE(!out_bot.eof() && !ref_bot.eof());

  std::istreambuf_iterator<char> out_bot_iter(out_bot);
  std::istreambuf_iterator<char> ref_bot_iter(ref_bot);
  std::istreambuf_iterator<char> eos_bot_iter;
  while (ref_bot_iter != eos_bot_iter && out_bot_iter != eos_bot_iter) {
    EXPECT_EQ(*ref_bot_iter++, *out_bot_iter++);
  }

  EXPECT_EQ(out_top_iter, eos_top_iter);
  EXPECT_EQ(out_bot_iter, eos_bot_iter);
  EXPECT_EQ(ref_top_iter, eos_top_iter);
  EXPECT_EQ(ref_bot_iter, eos_bot_iter);
}
