/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "usm_queries/usm_queries.hpp"
#include "test_harness/test_harness.hpp"

HWTEST(UsmQueriesSystemTests, ApplicationReturnsOKStatus) {
  compute_samples::UsmQueriesApplication application;
  std::vector<std::string> command_line = {};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

TEST(UsmQueriesSystemTests,
     ApplicationReturnsSkipStatusGivenHelpMessageIsRequested) {
  compute_samples::UsmQueriesApplication application;
  std::vector<std::string> command_line = {"--help"};
  EXPECT_EQ(compute_samples::Application::Status::SKIP,
            application.run(command_line));
}
