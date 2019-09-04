/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"

#include "utils/utils.hpp"
namespace cs = compute_samples;

template <typename T> class SizeInBytes : public testing::Test {};
typedef testing::Types<uint8_t, int8_t, uint16_t, int16_t, uint32_t, int16_t,
                       uint64_t, int64_t, float, double>
    StandardTypes;
TYPED_TEST_CASE(SizeInBytes, StandardTypes);

TYPED_TEST(SizeInBytes, EmptyVector) {
  const std::vector<TypeParam> vector(0);
  EXPECT_EQ(0, cs::size_in_bytes(vector));
}

TYPED_TEST(SizeInBytes, SingleElement) {
  const std::vector<TypeParam> vector(1);
  EXPECT_EQ(sizeof(TypeParam) * vector.size(), cs::size_in_bytes(vector));
}

TYPED_TEST(SizeInBytes, MultipleElements) {
  const std::vector<TypeParam> vector(2);
  EXPECT_EQ(sizeof(TypeParam) * vector.size(), cs::size_in_bytes(vector));
}

TEST(PackVector, Input8BitsOutput8BitsStep1) {
  const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03};
  const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03};
  const std::vector<uint8_t> output = cs::pack_vector<uint8_t>(input, 1);
  EXPECT_EQ(expected, output);
}

TEST(PackVector, Input8BitsOutput16BitsStep1) {
  const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03,
                                      0x04, 0x05, 0x06, 0x07};
  const std::vector<uint16_t> expected = {0x0100, 0x0302, 0x0504, 0x0706};
  const std::vector<uint16_t> output = cs::pack_vector<uint16_t>(input, 1);
  EXPECT_EQ(expected, output);
}

TEST(PackVector, Input8BitsOutput32BitsStep1) {
  const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                      0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                      0x0C, 0x0D, 0x0E, 0x0F};
  const std::vector<uint32_t> expected = {0x03020100, 0x07060504, 0x0B0A0908,
                                          0x0F0E0D0C};
  const std::vector<uint32_t> output = cs::pack_vector<uint32_t>(input, 1);
  EXPECT_EQ(expected, output);
}

TEST(PackVector, Input16BitsOutput16BitsStep1) {
  const std::vector<uint16_t> input = {0x0100, 0x0302, 0x0504, 0x0706};
  const std::vector<uint16_t> expected = {0x0100, 0x0302, 0x0504, 0x0706};
  const std::vector<uint16_t> output = cs::pack_vector<uint16_t>(input, 1);
  EXPECT_EQ(expected, output);
}

TEST(PackVector, Input16BitsOutput32BitsStep1) {
  const std::vector<uint16_t> input = {0x0100, 0x0302, 0x0504, 0x0706,
                                       0x0908, 0x0B0A, 0x0D0C, 0x0F0E};
  const std::vector<uint32_t> expected = {0x03020100, 0x07060504, 0x0B0A0908,
                                          0x0F0E0D0C};
  const std::vector<uint32_t> output = cs::pack_vector<uint32_t>(input, 1);
  EXPECT_EQ(expected, output);
}

TEST(PackVector, Input32BitsOutput32BitsStep1) {
  const std::vector<uint32_t> input = {0x03020100, 0x07060504, 0x0B0A0908,
                                       0x0F0E0D0C};
  const std::vector<uint32_t> expected = {0x03020100, 0x07060504, 0x0B0A0908,
                                          0x0F0E0D0C};
  const std::vector<uint32_t> output = cs::pack_vector<uint32_t>(input, 1);
  EXPECT_EQ(expected, output);
}

struct CustomType {
  uint8_t s[3];
};

TEST(PackVector, Input8BitsOutput24BitsStep1) {
  const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03, 0x04,
                                      0x05, 0x06, 0x07, 0x08};
  const std::vector<CustomType> expected = {
      {0x00, 0x01, 0x02}, {0x03, 0x04, 0x05}, {0x06, 0x07, 0x08}};
  const std::vector<CustomType> output = cs::pack_vector<CustomType>(input, 1);
  for (size_t i = 0; i < output.size(); ++i) {
    EXPECT_EQ(expected[i].s[0], output[i].s[0]);
    EXPECT_EQ(expected[i].s[1], output[i].s[1]);
    EXPECT_EQ(expected[i].s[2], output[i].s[2]);
  }
}

TEST(PackVector, Input8BitsOutput8BitsStep2) {
  const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03};
  const std::vector<uint8_t> expected = {0x00, 0x01, 0x02, 0x03};
  const std::vector<uint8_t> output = cs::pack_vector<uint8_t>(input, 2);
  EXPECT_EQ(expected, output);
}

TEST(PackVector, Input8BitsOutput16BitsStep2) {
  const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03,
                                      0x04, 0x05, 0x06, 0x07};
  const std::vector<uint16_t> expected = {0x0200, 0x0301, 0x0604, 0x0705};
  const std::vector<uint16_t> output = cs::pack_vector<uint16_t>(input, 2);
  EXPECT_EQ(expected, output);
}

TEST(PackVector, Input8BitsOutput32BitsStep2) {
  const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                      0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                      0x0C, 0x0D, 0x0E, 0x0F};
  const std::vector<uint32_t> expected = {0x06040200, 0x07050301, 0x0E0C0A08,
                                          0x0F0D0B09};
  const std::vector<uint32_t> output = cs::pack_vector<uint32_t>(input, 2);
  EXPECT_EQ(expected, output);
}

TEST(PackVector, Input8BitsOutput16BitsStep3) {
  const std::vector<uint8_t> input = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                      0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
  const std::vector<uint16_t> expected = {0x0300, 0x0401, 0x0502,
                                          0x0906, 0x0A07, 0xB08};
  const std::vector<uint16_t> output = cs::pack_vector<uint16_t>(input, 3);
  EXPECT_EQ(expected, output);
}
