/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_APPLICATION_HPP
#define COMPUTE_SAMPLES_APPLICATION_HPP

#include <vector>
#include <string>

namespace compute_samples {
class Application {
public:
  enum class Status { OK = 0, ERROR = 1, SKIP = 2 };
  virtual ~Application() = default;
  Status run(std::vector<std::string> &command_line);

private:
  virtual Status run_implementation(std::vector<std::string> &command_line) = 0;
};
} // namespace compute_samples

#endif
