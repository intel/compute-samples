/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "fp_types/half.hpp"
#include "fp_types/common.hpp"
#include "gtest/gtest.h"
#include "utils/utils.hpp"

#include <limits>

namespace cs = compute_samples;

namespace {

// 0.000000059604645 subnormal gets converted wrong
// (to 0x0000, should be 0x0001) in two cases:
// float -> uint16
// float -> uint16 -> float
const std::vector<float> float_values{{0.0f, -0.0f, 1.0f, 2.0f, -0.1572265625f,
                                       std::numeric_limits<float>::infinity()}};
const std::vector<float> float_expected{
    {0.0f, -0.0f, 1.0f, 2.0f, -0.1572265625f,
     std::numeric_limits<float>::infinity()}};
const std::vector<float> float_rnde_rounding_values{{
    cs::uint32_to_float(0x42005000), // down
    cs::uint32_to_float(0x42007000), // up
    cs::uint32_to_float(0xffffffff)  // max
}};
const std::vector<uint16_t> half_values{
    {0x0000, 0x8000, 0x3c00, 0x4000, 0xb108, 0x7c00}};
const std::vector<uint16_t> half_expected{
    {0x0000, 0x8000, 0x3c00, 0x4000, 0xb108, 0x7c00}};
const std::vector<uint16_t> half_rnde_rounding_expected{
    {0x5002, 0x5004, 0xffff}};

template <typename In, typename Out>
struct ConvertTest : public testing::TestWithParam<std::tuple<In, Out>> {
  typedef cs::PrintToHexStringParamName<In> test_name_suffix_type;

  void test(In value, Out expected) {
    auto result = cs::convert<In, cs::half, Out>(value);
    if (std::is_same<Out, float>::value) {
      uint32_t result_u32 = *reinterpret_cast<uint32_t *>(&result);
      uint32_t expected_u32 = *reinterpret_cast<uint32_t *>(&expected);
      EXPECT_EQ(result_u32, expected_u32);
    } else {
      EXPECT_EQ(result, expected);
    }
  }
};

class HalfFromUint16AsUint16 : public ConvertTest<uint16_t, uint16_t> {};
class HalfFromFloatAsFloat : public ConvertTest<float, float> {};

class HalfFromUint16AsFloat : public ConvertTest<uint16_t, float> {};
class HalfFromFloatAsUint16 : public ConvertTest<float, uint16_t> {};

class HalfFromFloatRndeRounding : public ConvertTest<float, uint16_t> {};

TEST_P(HalfFromUint16AsUint16, Convert) {
  test(std::get<0>(GetParam()), std::get<1>(GetParam()));
}
TEST_P(HalfFromFloatAsFloat, Convert) {
  test(std::get<0>(GetParam()), std::get<1>(GetParam()));
}

TEST_P(HalfFromUint16AsFloat, Convert) {
  test(std::get<0>(GetParam()), std::get<1>(GetParam()));
}
TEST_P(HalfFromFloatAsUint16, Convert) {
  test(std::get<0>(GetParam()), std::get<1>(GetParam()));
}

TEST_P(HalfFromFloatRndeRounding, Convert) {
  test(std::get<0>(GetParam()), std::get<1>(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(Half, HalfFromUint16AsUint16,
                         testing::ValuesIn(cs::zip(half_values, half_values)),
                         HalfFromUint16AsUint16::test_name_suffix_type());
INSTANTIATE_TEST_SUITE_P(Half, HalfFromFloatAsFloat,
                         testing::ValuesIn(cs::zip(float_expected,
                                                   float_expected)),
                         HalfFromFloatAsFloat::test_name_suffix_type());

INSTANTIATE_TEST_SUITE_P(Half, HalfFromUint16AsFloat,
                         testing::ValuesIn(cs::zip(half_values,
                                                   float_expected)),
                         HalfFromUint16AsFloat::test_name_suffix_type());
INSTANTIATE_TEST_SUITE_P(Half, HalfFromFloatAsUint16,
                         testing::ValuesIn(cs::zip(float_values,
                                                   half_expected)),
                         HalfFromFloatAsUint16::test_name_suffix_type());

INSTANTIATE_TEST_SUITE_P(
    Half, HalfFromFloatRndeRounding,
    testing::ValuesIn(cs::zip(float_rnde_rounding_values,
                              half_rnde_rounding_expected)),
    HalfFromFloatRndeRounding::test_name_suffix_type());

TEST(Half, Negation) {
  const float x = 1.5f;
  const cs::half x_hf(x);
  const float expected = -x;
  const float actual = static_cast<float>(-x_hf);
  EXPECT_NEAR(expected, actual, 1e-2);
}

TEST(Half, Multiplication) {
  const float x = 1.5f;
  const float y = -2.0f;
  const cs::half x_hf(x);
  const cs::half y_hf(y);
  const float expected = x * y;
  const float actual = static_cast<float>(x_hf * y_hf);
  EXPECT_NEAR(expected, actual, 1e-2);
}

TEST(Half, Addition) {
  const float x = 1.5f;
  const float y = -2.0f;
  const cs::half x_hf(x);
  const cs::half y_hf(y);
  const float expected = x + y;
  const float actual = static_cast<float>(x_hf + y_hf);
  EXPECT_NEAR(expected, actual, 1e-2);
}

TEST(Half, NanSensitiveComparisonSameNanValue) {
  const cs::half h(uint16_t(0x7f00));
  EXPECT_TRUE(h.nan_sensitive_eq(h));
}

TEST(Half, NanSensitiveComparisonDifferentNanValue) {
  const cs::half h1(uint16_t(0x7f00));
  const cs::half h2(uint16_t(0x7c01));
  EXPECT_TRUE(h1.nan_sensitive_eq(h2));
}

TEST(Half, NanSensitiveComparisonNanInf) {
  const cs::half h1(uint16_t(0x7f00));
  const cs::half h2(uint16_t(0x7c00));
  EXPECT_FALSE(h1.nan_sensitive_eq(h2));
}

} // namespace
