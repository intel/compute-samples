/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "version/version.hpp"
#include "version_config.hpp"
#include <sstream>
#include <string>

namespace compute_samples {
std::string get_version_string() {
  std::stringstream ss;
  ss << major_version << "." << minor_version << "." << patch_version << "+"
     << build_number << "." << git_commit;
  return ss.str();
}
} // namespace compute_samples
