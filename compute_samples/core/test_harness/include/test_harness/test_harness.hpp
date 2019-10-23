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

struct TestHarnessSettings {
  bool allow_skips = false;
};

void init_test_harness();
void init_test_harness(const TestHarnessSettings settings);
void init_test_harness(std::vector<std::string> &command_line);
TestHarnessSettings
test_harness_parse_command_line(std::vector<std::string> &command_line);

void report_unsupported_scenario(const std::string &message);

} // namespace compute_samples

#endif
