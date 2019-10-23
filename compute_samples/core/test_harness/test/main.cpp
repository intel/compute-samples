/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "logging/logging.hpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::vector<std::string> command_line(argv + 1, argv + argc);
  compute_samples::init_logging(command_line);
  return RUN_ALL_TESTS();
}
