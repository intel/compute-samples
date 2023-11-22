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

#include "vme_interlaced/vme_interlaced.hpp"
#include "test_harness/test_harness.hpp"
#include "logging/logging.hpp"

class VmeInterlacedSystemTests : public testing::Test {
protected:
  void TearDown() override {
    if (std::remove(output_top_file_.c_str()) != 0) {
      LOG_DEBUG << "Deleting file " << output_top_file_.c_str() << " failed";
    }
    if (std::remove(output_bot_file_.c_str()) != 0) {
      LOG_DEBUG << "Deleting file " << output_bot_file_.c_str() << " failed";
    }
    if (std::remove(reference_top_file_.c_str()) != 0) {
      LOG_DEBUG << "Deleting file " << reference_top_file_.c_str() << " failed";
    }
    if (std::remove(reference_bot_file_.c_str()) != 0) {
      LOG_DEBUG << "Deleting file " << reference_bot_file_.c_str() << " failed";
    }
  }

  const std::string input_file_ = "football_interlaced_720x480.yuv";
  const std::string output_top_file_ =
      "output_top_football_interlaced_720x480.yuv";
  const std::string output_bot_file_ =
      "output_bot_football_interlaced_720x480.yuv";
  const std::string reference_top_file_ = "top_football_interlaced_720x480.yuv";
  const std::string reference_bot_file_ = "bot_football_interlaced_720x480.yuv";
};

HWTEST_F(VmeInterlacedSystemTests, ReturnsReferenceImage) {
  std::vector<std::string> command_line = {input_file_,
                                           output_top_file_,
                                           output_bot_file_,
                                           "--width",
                                           "720",
                                           "--height",
                                           "480",
                                           "-s",
                                           "native",
                                           "-f",
                                           "30"};

  compute_samples::VmeInterlacedApplication application;

  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

  static const int reference_top_file_pos = 1;
  static const int reference_bot_file_pos = reference_top_file_pos + 1;
  static const int sub_test_pos = reference_top_file_pos + 7;

  command_line[reference_top_file_pos] = reference_top_file_;
  command_line[reference_bot_file_pos] = reference_bot_file_;
  command_line[sub_test_pos] = "split";

  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

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
