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

#include "vme_intra/vme_intra.hpp"
#include "test_harness/test_harness.hpp"
#include "logging/logging.hpp"

class VmeIntraSystemTests : public testing::Test {
protected:
  void TearDown() override {
    if (std::remove(output_file_.c_str()) != 0) {
      LOG_DEBUG << "Deleting file " << output_file_.c_str() << " failed";
    }
  }

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
