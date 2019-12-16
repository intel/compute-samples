/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "fp_types/half.hpp"
#include "gtest/gtest.h"

#include <cstring>

namespace cs = compute_samples;

namespace {

TEST(Half, FromUint16AsUint16) {
  uint16_t v = 0xb108; // 0b1011000100001000
  cs::half h(v);
  EXPECT_EQ(v, static_cast<uint16_t>(h));
}

TEST(Half, FromUint16AsFloat) {
  uint16_t v = 0xb108; // 0b1011000100001000
  cs::half h(v);
  EXPECT_FLOAT_EQ(-0.1572265625, static_cast<float>(h));
}

TEST(Half, FromFloatAsUint16) {
  float v = -0.1572265625f; // 0b1011000100001000
  cs::half h(v);
  EXPECT_EQ(uint16_t(0xb108), static_cast<uint16_t>(h));
}

TEST(Half, FromFloatAsFloat) {
  float v = -0.1572265625f; // 0b1011000100001000
  cs::half h(v);
  EXPECT_FLOAT_EQ(v, static_cast<float>(h));
}

TEST(Half, FromFloatRndeRoundingDown) {
  float v;
  uint32_t tmp = 0x42005000;
  std::memcpy(&v, &tmp, sizeof(v));
  cs::half h(v);
  EXPECT_EQ(uint16_t(0x5002), static_cast<uint16_t>(h));
}

TEST(Half, FromFloatRndeRoundingUp) {
  float v;
  uint32_t tmp = 0x42007000;
  std::memcpy(&v, &tmp, sizeof(v));
  cs::half h(v);
  EXPECT_EQ(uint16_t(0x5004), static_cast<uint16_t>(h));
}

TEST(Half, FromFloatRndeRoundingMaxUint32) {
  float v;
  uint32_t tmp = 0xffffffff;
  std::memcpy(&v, &tmp, sizeof(v));
  cs::half h(v);
  EXPECT_EQ(uint16_t(0xffff), static_cast<uint16_t>(h));
}

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

} // namespace
