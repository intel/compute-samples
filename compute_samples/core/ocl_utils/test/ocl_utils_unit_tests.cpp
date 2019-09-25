/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#include "ocl_utils/ocl_utils.hpp"
#include "ocl_utils/unified_shared_memory.hpp"
#include "gtest/gtest.h"
#include "utils/utils.hpp"

namespace cs = compute_samples;
namespace compute = boost::compute;

template <typename T> class SizeInBytes : public testing::Test {};
typedef testing::Types<cl_char, cl_uchar, cl_short, cl_ushort, cl_int, cl_uint,
                       cl_long, cl_ulong, cl_half, cl_float, cl_double>
    OpenCLTypes;
TYPED_TEST_CASE(SizeInBytes, OpenCLTypes);

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

TEST(CompareCLVectors, cl_int8_equal) {
  const cl_int8 lhs = {0, 1, 2, 3, 4, 5, 6, 7};
  const cl_int8 rhs = lhs;
  EXPECT_TRUE(compute_samples::compare_cl_vectors(lhs, rhs));
}

TEST(CompareCLVectors, cl_int8_not_equal) {
  const cl_int8 lhs = {0, 1, 2, 3, 4, 5, 6, 7};
  const cl_int8 rhs = {1, 2, 3, 4, 5, 6, 7, 0};
  EXPECT_FALSE(compute_samples::compare_cl_vectors(lhs, rhs));
}

TEST(CompareCLVectors, cl_int4_equal) {
  const cl_int4 lhs = {0, 1, 2, 3};
  const cl_int4 rhs = lhs;
  EXPECT_TRUE(compute_samples::compare_cl_vectors(lhs, rhs));
}

TEST(CompareCLVectors, cl_int4_not_equal) {
  const cl_int4 lhs = {0, 1, 2, 3};
  const cl_int4 rhs = {1, 2, 3, 4};
  EXPECT_FALSE(compute_samples::compare_cl_vectors(lhs, rhs));
}

TEST(CompareCLVectors, cl_int2_equal) {
  const cl_int2 lhs = {0, 1};
  const cl_int2 rhs = lhs;
  EXPECT_TRUE(compute_samples::compare_cl_vectors(lhs, rhs));
}

TEST(CompareCLVectors, cl_int2_not_equal) {
  const cl_int2 lhs = {0, 1};
  const cl_int2 rhs = {1, 2};
  EXPECT_FALSE(compute_samples::compare_cl_vectors(lhs, rhs));
}

TEST(CLVectorToString, cl_int8) {
  const cl_int8 x = {0, 1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ("[0, 1, 2, 3, 4, 5, 6, 7]",
            compute_samples::cl_vector_to_string(x));
}

TEST(CLVectorToString, cl_int4) {
  const cl_int4 x = {0, 1, 2, 3};
  EXPECT_EQ("[0, 1, 2, 3]", compute_samples::cl_vector_to_string(x));
}

TEST(CLVectorToString, cl_int2) {
  const cl_int2 x = {0, 1};
  EXPECT_EQ("[0, 1]", compute_samples::cl_vector_to_string(x));
}

TEST(ToStringTest, UsmTypeHost) {
  const compute::usm_type t = compute::usm_type::host;
  const std::string expected = "host";
  EXPECT_EQ(expected, cs::to_string(t));
}

TEST(ToStringTest, UsmTypeDevice) {
  const compute::usm_type t = compute::usm_type::device;
  const std::string expected = "device";
  EXPECT_EQ(expected, cs::to_string(t));
}

TEST(ToStringTest, UsmTypeShared) {
  const compute::usm_type t = compute::usm_type::shared;
  const std::string expected = "shared";
  EXPECT_EQ(expected, cs::to_string(t));
}
