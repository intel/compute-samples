/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "usm_hello_world/usm_hello_world.hpp"
#include "test_harness/test_harness.hpp"

TEST(UsmHelloWorldSystemTests,
     ApplicationReturnsSkipStatusGivenHelpMessageIsRequested) {
  compute_samples::UsmHelloWorldApplication application;
  std::vector<std::string> command_line = {"--help"};
  EXPECT_EQ(compute_samples::Application::Status::SKIP,
            application.run(command_line));
}

HWTEST(UsmHelloWorldSystemTests, GivenHostUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmHelloWorldApplication application;
  std::vector<std::string> command_line = {"host"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmHelloWorldSystemTests, GivenDeviceUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmHelloWorldApplication application;
  std::vector<std::string> command_line = {"device"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmHelloWorldSystemTests, GivenSharedUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmHelloWorldApplication application;
  std::vector<std::string> command_line = {"shared"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmHelloWorldSystemTests,
       GivenCustomAllocationSizeThenApplicationReturnsOKStatus) {
  compute_samples::UsmHelloWorldApplication application;
  std::vector<std::string> command_line = {"host", "--size", "1024"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}
