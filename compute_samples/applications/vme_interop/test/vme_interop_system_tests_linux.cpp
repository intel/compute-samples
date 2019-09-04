/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_interop/vme_interop.hpp"
#include "vme_interop/vme_interop_linux.hpp"
#include "vme_interop_system_tests_common.hpp"
#include "test_harness/test_harness.hpp"

HWTEST_F(VmeInteropSystemTests, ReturnsReferenceImage) {
  std::vector<std::string> command_line = {input_file_, output_file_, "--width",
                                           "176",       "--height",   "144",
                                           "-f",        "50"};

  compute_samples::VmeInteropApplication application;
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));

  std::ifstream out(output_file_, std::ios::binary);
  std::string reference_file = "interop_";
  reference_file += input_file_;
  std::ifstream ref(reference_file, std::ios::binary);
  EXPECT_TRUE(out.good() && ref.good());
  EXPECT_TRUE(!out.eof() && !ref.eof());

  std::istreambuf_iterator<char> out_iter(out);
  std::istreambuf_iterator<char> ref_iter(ref);
  std::istreambuf_iterator<char> eos_iter;
  while (ref_iter != eos_iter) {
    EXPECT_EQ(*ref_iter++, *out_iter++);
  }

  EXPECT_EQ(out_iter, eos_iter);
}
