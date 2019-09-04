/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "logging/logging.hpp"

namespace compute_samples {

int add_numbers(int a, int b) {
  int sum = a + b;
  LOG_INFO << "Numbers Added";
  return sum;
}

} // namespace compute_samples
