/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "ze_info/ze_info.hpp"
#include "test_harness/test_harness.hpp"

HWTEST(ZeInfoSystemTests, ApplicationReturnsOKStatus) {
  compute_samples::ZeInfoApplication application;
  std::vector<std::string> command_line = {};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

TEST(ZeInfoSystemTests,
     ApplicationReturnsSkipStatusGivenHelpMessageIsRequested) {
  compute_samples::ZeInfoApplication application;
  std::vector<std::string> command_line = {"--help"};
  EXPECT_EQ(compute_samples::Application::Status::SKIP,
            application.run(command_line));
}

class ZeInfoSystemTestsWithOutputFile : public testing::Test {
protected:
  void TearDown() override { std::remove(output_file_.c_str()); }

  const std::string output_file_ = "test_output.json";
};

HWTEST_F(ZeInfoSystemTestsWithOutputFile, ApplicationSavesOutputToFile) {
  compute_samples::ZeInfoApplication application;
  std::vector<std::string> command_line = {"--output", output_file_};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}
