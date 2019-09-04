/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_interop/vme_interop.hpp"
#include "vme_interop_system_tests_common.hpp"
#include "test_harness/test_harness.hpp"

HWTEST_F(VmeInteropSystemTests, ReturnsSkipStatusOnWindows) {
  std::vector<std::string> command_line = {input_file_, output_file_, "--width",
                                           "176",       "--height",   "144",
                                           "-f",        "50"};

  compute_samples::VmeInteropApplication application;
  EXPECT_EQ(compute_samples::Application::Status::SKIP,
            application.run(command_line));
}
