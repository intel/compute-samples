/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "application/application.hpp"

#include "version/version.hpp"
#include "logging/logging.hpp"

namespace compute_samples {
Application::Status Application::run(std::vector<std::string> &command_line) {
  try {
    LOG_INFO << "Version: " << get_version_string();
    return run_implementation(command_line);
  } catch (const std::exception &e) {
    LOG_FATAL << e.what();
    return Status::ERROR;
  }
}
} // namespace compute_samples
