/*
 * Copyright(c) 2019 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "test_harness/test_harness.hpp"
#include "gtest/gtest.h"

namespace cs = compute_samples;

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
