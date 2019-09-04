/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "usm_linked_list/usm_linked_list.hpp"
#include "test_harness/test_harness.hpp"

TEST(UsmLinkedListSystemTests,
     ApplicationReturnsSkipStatusGivenHelpMessageIsRequested) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"--help"};
  EXPECT_EQ(compute_samples::Application::Status::SKIP,
            application.run(command_line));
}

HWTEST(UsmLinkedListSystemTests, GivenHostUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"host"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmLinkedListSystemTests, GivenDeviceUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"device"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmLinkedListSystemTests, GivenSharedUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"shared"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmLinkedListSystemTests,
       GivenCustomListSizeThenApplicationReturnsOKStatus) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"host", "--size", "1024"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}
