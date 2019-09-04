/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "usm_mem_info/usm_mem_info.hpp"
#include "test_harness/test_harness.hpp"

TEST(UsmMemInfoSystemTests,
     ApplicationReturnsSkipStatusGivenHelpMessageIsRequested) {
  compute_samples::UsmMemInfoApplication application;
  std::vector<std::string> command_line = {"--help"};
  EXPECT_EQ(compute_samples::Application::Status::SKIP,
            application.run(command_line));
}

HWTEST(UsmMemInfoSystemTests, GivenHostUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmMemInfoApplication application;
  std::vector<std::string> command_line = {"host"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmMemInfoSystemTests, GivenDeviceUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmMemInfoApplication application;
  std::vector<std::string> command_line = {"device"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmMemInfoSystemTests, GivenSharedUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmMemInfoApplication application;
  std::vector<std::string> command_line = {"shared"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}
