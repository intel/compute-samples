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

#ifndef _WIN32
#define PUTENV putenv
#else
#define PUTENV _putenv
#endif
    char set_default_vendor[] = "BOOST_COMPUTE_DEFAULT_VENDOR=Intel";
    char set_default_device[] = "BOOST_COMPUTE_DEFAULT_DEVICE_TYPE=GPU";
    PUTENV(set_default_vendor);
    PUTENV(set_default_device);

    return run_implementation(command_line);
  } catch (const std::exception &e) {
    LOG_FATAL << e.what();
    return Status::ERROR;
  }
}
} // namespace compute_samples
