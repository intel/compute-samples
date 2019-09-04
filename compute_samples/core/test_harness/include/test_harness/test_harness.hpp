/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_TEST_HARNESS_HPP
#define COMPUTE_SAMPLES_TEST_HARNESS_HPP

namespace compute_samples {

#define HWTEST(test_case_name, test_name)                                      \
  TEST(test_case_name, test_name##_HWTEST)
#define HWTEST_F(test_case_name, test_name)                                    \
  TEST_F(test_case_name, test_name##_HWTEST)
#define HWTEST_P(test_case_name, test_name)                                    \
  TEST_P(test_case_name, test_name##_HWTEST)
#define TYPED_HWTEST(test_case_name, test_name)                                \
  TYPED_TEST(test_case_name, test_name##_HWTEST)

} // namespace compute_samples

#endif
