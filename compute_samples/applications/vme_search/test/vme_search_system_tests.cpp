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

#include "vme_search/vme_search.hpp"
#include "test_harness/test_harness.hpp"

class VmeSearchSystemTests : public testing::Test {
protected:
  virtual void TearDown() { std::remove(output_file_.c_str()); }

  void verify(std::string sub_test) {
    std::vector<std::string> command_line = {
        input_file_, output_file_, "--width", "176",    "--height", "144",
        "--qp",      "45",         "-s",      sub_test, "-f",       "50"};

    compute_samples::VmeSearchApplication application;
    application.run(command_line);

    std::ifstream out(output_file_, std::ios::binary);
    std::string reference_file = sub_test;
    reference_file += "_";
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

  const std::string input_file_ = "foreman_176x144.yuv";
  const std::string output_file_ = "output_foreman_176x144.yuv";
};

HWTEST_F(VmeSearchSystemTests, BasicSearch) { verify("basic_search"); }

HWTEST_F(VmeSearchSystemTests, CostHeuristicsSearch) {
  verify("cost_heuristics_search");
}

HWTEST_F(VmeSearchSystemTests, LargerSearch) { verify("larger_search"); }
