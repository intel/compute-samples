/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gmock/gmock.h"
#include "logging/logging.hpp"
#include "test_harness/test_harness.hpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  std::vector<std::string> command_line(argv + 1, argv + argc);
  compute_samples::init_logging(command_line);
  compute_samples::init_test_harness(command_line);
  return RUN_ALL_TESTS();
}
