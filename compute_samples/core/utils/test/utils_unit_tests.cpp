/*
 * Copyright(c) 2018 Intel Corporation
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

#include "utils/utils.hpp"
#include "gtest/gtest.h"

template <typename T> class SizeInBytes : public testing::Test {};
typedef testing::Types<uint8_t, int8_t, uint16_t, int16_t, uint32_t, int16_t,
                       uint64_t, int64_t, float, double>
    StandardTypes;
TYPED_TEST_CASE(SizeInBytes, StandardTypes);

TYPED_TEST(SizeInBytes, EmptyVector) {
  const std::vector<TypeParam> vector(0);
  EXPECT_EQ(0, compute_samples::size_in_bytes(vector));
}

TYPED_TEST(SizeInBytes, SingleElement) {
  const std::vector<TypeParam> vector(1);
  EXPECT_EQ(sizeof(TypeParam) * vector.size(),
            compute_samples::size_in_bytes(vector));
}

TYPED_TEST(SizeInBytes, MultipleElements) {
  const std::vector<TypeParam> vector(2);
  EXPECT_EQ(sizeof(TypeParam) * vector.size(),
            compute_samples::size_in_bytes(vector));
}
