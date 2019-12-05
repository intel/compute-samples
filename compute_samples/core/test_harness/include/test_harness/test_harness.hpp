/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_TEST_HARNESS_HPP
#define COMPUTE_SAMPLES_TEST_HARNESS_HPP

#include <string>
#include <vector>

namespace compute_samples {

#define HWTEST(test_case_name, test_name)                                      \
  TEST(test_case_name, test_name##_HWTEST)
#define HWTEST_F(test_case_name, test_name)                                    \
  TEST_F(test_case_name, test_name##_HWTEST)
#define HWTEST_P(test_case_name, test_name)                                    \
  TEST_P(test_case_name, test_name##_HWTEST)
#define TYPED_HWTEST(test_case_name, test_name)                                \
  TYPED_TEST(test_case_name, test_name##_HWTEST)

// This macro is required, because GTEST_* macros do not work outside of gtest
// classes
#define REPORT_UNSUPPORTED_SCENARIO(message)                                   \
  {                                                                            \
    if (compute_samples::are_skips_allowed()) {                                \
      GTEST_SKIP() << message;                                                 \
    }                                                                          \
    GTEST_FAIL() << message                                                    \
                 << "\nPass --allow-skips flag to set unsupported "            \
                    "scenarios as skips";                                      \
  }

struct TestHarnessSettings {
  bool allow_skips = false;
};

void init_test_harness();
void init_test_harness(const TestHarnessSettings settings);
void init_test_harness(std::vector<std::string> &command_line);
TestHarnessSettings
test_harness_parse_command_line(std::vector<std::string> &command_line);

bool are_skips_allowed();

} // namespace compute_samples

#endif
