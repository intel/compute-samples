/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "template/template.hpp"
#include "test_harness/test_harness.hpp"

HWTEST(TemplateSystemTests, ApplicationReturnsOKStatus) {
  compute_samples::TemplateApplication application;
  std::vector<std::string> command_line = {};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

TEST(TemplateSystemTests,
     ApplicationReturnsSkipStatusGivenHelpMessageIsRequested) {
  compute_samples::TemplateApplication application;
  std::vector<std::string> command_line = {"--help"};
  EXPECT_EQ(compute_samples::Application::Status::SKIP,
            application.run(command_line));
}
