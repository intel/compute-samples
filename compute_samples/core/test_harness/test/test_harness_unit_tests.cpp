/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_harness/test_harness.hpp"
#include "gtest/gtest.h"

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
