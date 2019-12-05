/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "random/random.hpp"
#include "gtest/gtest.h"

#include <limits>

namespace cs = compute_samples;

typedef testing::Types<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t,
                       uint32_t, uint64_t>
    IntegerTypes;
typedef testing::Types<float, double> FloatingPointTypes;
typedef testing::Types<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t,
                       uint32_t, uint64_t, float, double>
    StandardTypes;

template <typename T> class GenerateValue : public testing::Test {
protected:
  const int iterations = 100;
  const int seed = 0;
};
TYPED_TEST_SUITE(GenerateValue, StandardTypes);

template <typename T> class GenerateIntegerValue : public GenerateValue<T> {};
TYPED_TEST_SUITE(GenerateIntegerValue, IntegerTypes);

template <typename T>
class GenerateFloatingPointValue : public GenerateValue<T> {};
TYPED_TEST_SUITE(GenerateFloatingPointValue, FloatingPointTypes);

TYPED_TEST(GenerateIntegerValue, WithinGivenMinAndMaxValue) {
  const TypeParam min = 1;
  const TypeParam max = 10;
  for (int i = 0; i < this->iterations; ++i) {
    const TypeParam value = cs::generate_value(min, max, this->seed);
    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
  }
}

TYPED_TEST(GenerateFloatingPointValue, WithinGivenMinAndMaxValue) {
  const TypeParam min = 1.5;
  const TypeParam max = 10.5;
  for (int i = 0; i < this->iterations; ++i) {
    const TypeParam value = cs::generate_value(min, max, this->seed);
    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
  }
}

TYPED_TEST(GenerateValue, WithinTypeMinAndMaxValue) {
  const TypeParam min = std::numeric_limits<TypeParam>::min();
  const TypeParam max = std::numeric_limits<TypeParam>::max();
  for (int i = 0; i < this->iterations; ++i) {
    const auto value = cs::generate_value<TypeParam>(this->seed);
    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
  }
}

template <typename T> class GenerateVector : public testing::Test {
protected:
  const int size = 100;
  const int seed = 0;
};
TYPED_TEST_SUITE(GenerateVector, StandardTypes);

template <typename T> class GenerateIntegerVector : public GenerateVector<T> {};
TYPED_TEST_SUITE(GenerateIntegerVector, IntegerTypes);

template <typename T>
class GenerateFloatingPointVector : public GenerateVector<T> {};
TYPED_TEST_SUITE(GenerateFloatingPointVector, FloatingPointTypes);

TYPED_TEST(GenerateIntegerVector, WithinGivenMinAndMaxValue) {
  const TypeParam min = 1;
  const TypeParam max = 10;
  const std::vector<TypeParam> vector =
      cs::generate_vector(this->size, min, max, this->seed);
  for (const auto &value : vector) {
    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
  }
}

TYPED_TEST(GenerateFloatingPointVector, WithinGivenMinAndMaxValue) {
  const TypeParam min = 1.5;
  const TypeParam max = 10.5;
  const std::vector<TypeParam> vector =
      cs::generate_vector(this->size, min, max, this->seed);
  for (const auto &value : vector) {
    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
  }
}

TYPED_TEST(GenerateVector, WithinTypeMinAndMaxValue) {
  const TypeParam min = std::numeric_limits<TypeParam>::min();
  const TypeParam max = std::numeric_limits<TypeParam>::max();
  const std::vector<TypeParam> vector =
      cs::generate_vector<TypeParam>(this->size, this->seed);
  for (const auto &value : vector) {
    EXPECT_GE(value, min);
    EXPECT_LE(value, max);
  }
}

TYPED_TEST(GenerateVector, WithExpectedSize) {
  const std::vector<TypeParam> vector =
      cs::generate_vector<TypeParam>(this->size, this->seed);
  EXPECT_EQ(this->size, vector.size());
}
