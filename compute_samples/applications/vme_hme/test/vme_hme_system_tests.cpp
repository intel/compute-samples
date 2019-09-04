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

#include "vme_hme/vme_hme.hpp"
#include "test_harness/test_harness.hpp"

class VmeHmeSystemTests : public testing::Test {
protected:
  virtual void TearDown() { std::remove(output_file_.c_str()); }

  const std::string input_file_ = "foreman_176x144.yuv";
  const std::string output_file_ = "output_foreman_176x144.yuv";
};

HWTEST_F(VmeHmeSystemTests, ReturnsReferenceImage) {
  std::vector<std::string> command_line = {
      input_file_, output_file_, "--width", "176", "--height",
      "144",       "--qp",       "45",      "-f",  "50"};

  compute_samples::VmeHmeApplication application;
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

  std::ifstream out(output_file_, std::ios::binary);
  std::string reference_file = "hme_";
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
