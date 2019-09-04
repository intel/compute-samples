/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_intra/vme_intra.hpp"
#include "logging/logging.hpp"

int main(int argc, const char **argv) {
  std::vector<std::string> command_line(argv + 1, argv + argc);
  compute_samples::init_logging(command_line);
  compute_samples::VmeIntraApplication application;
  return static_cast<int>(application.run(command_line));
}
