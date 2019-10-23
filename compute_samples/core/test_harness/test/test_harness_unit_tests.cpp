/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_harness/test_harness.hpp"
#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

namespace cs = compute_samples;

namespace {

HWTEST(Test, TestCaseName) {
  const ::testing::TestInfo *const test_info =
      ::testing::UnitTest::GetInstance()->current_test_info();
  const std::string test_name = test_info->name();
  EXPECT_EQ("TestCaseName_HWTEST", test_name);
}

class TestFixture : public ::testing::Test {};

HWTEST_F(TestFixture, TestCaseName) {
  const ::testing::TestInfo *const test_info =
      ::testing::UnitTest::GetInstance()->current_test_info();
  const std::string test_name = test_info->name();
  EXPECT_EQ("TestCaseName_HWTEST", test_name);
}

class ValueParametrizedTest : public ::testing::TestWithParam<int> {};

HWTEST_P(ValueParametrizedTest, TestCaseName) {
  const ::testing::TestInfo *const test_info =
      ::testing::UnitTest::GetInstance()->current_test_info();
  const std::string test_name = test_info->name();
  EXPECT_EQ("TestCaseName_HWTEST/0", test_name);
}

INSTANTIATE_TEST_CASE_P(Dummy, ValueParametrizedTest, ::testing::Values(1));

template <typename T> class TypedTest : public ::testing::Test {};

using MyTypes = ::testing::Types<int>;
TYPED_TEST_CASE(TypedTest, MyTypes);

TYPED_HWTEST(TypedTest, TestCaseName) {
  const ::testing::TestInfo *const test_info =
      ::testing::UnitTest::GetInstance()->current_test_info();
  const std::string test_name = test_info->name();
  EXPECT_EQ("TestCaseName_HWTEST", test_name);
}

TEST(TestHarnessCommandLineParser, AllowSkipsDisabled) {
  std::vector<std::string> cmd;
  const cs::TestHarnessSettings settings =
      cs::test_harness_parse_command_line(cmd);
  EXPECT_FALSE(settings.allow_skips);
}

TEST(TestHarnessCommandLineParser, AllowSkipsEnabled) {
  std::vector<std::string> cmd = {"--allow-skips"};
  const cs::TestHarnessSettings settings =
      cs::test_harness_parse_command_line(cmd);
  EXPECT_TRUE(settings.allow_skips);
}

TEST(TestHarnessCommandLineParser, ConsumeOnlyKnownOptionsFromCommandLine) {
  std::vector<std::string> cmd = {"--allow-skips", "positional_option",
                                  "--option"};
  cs::test_harness_parse_command_line(cmd);
  EXPECT_EQ(2, cmd.size());
}

TEST(TestReportUnsupportedScenario, AllowSkipsDisabled) {
  cs::TestHarnessSettings settings;
  settings.allow_skips = false;
  cs::init_test_harness(settings);
  EXPECT_FATAL_FAILURE(cs::report_unsupported_scenario("message"), "message");
}

TEST(TestReportUnsupportedScenario, AllowSkipsEnabled) {
  cs::TestHarnessSettings settings;
  settings.allow_skips = true;
  cs::init_test_harness(settings);
  cs::report_unsupported_scenario("message");
  // GTEST_FAIL() << "Should not reach here";
}

} // namespace
