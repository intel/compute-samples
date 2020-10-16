/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ocl_utils/unified_shared_memory.hpp"

namespace compute = boost::compute;

namespace compute_samples {
std::string to_string(const compute::usm_type &x) {
  if (x == compute::usm_type::host) {
    return "host";
  }
  if (x == compute::usm_type::device) {
    return "device";
  }
  if (x == compute::usm_type::shared) {
    return "shared";
  }
  return "unknown";
}
} // namespace compute_samples

namespace boost {
namespace compute {
std::ostream &operator<<(std::ostream &os, const compute::usm_type &x) {
  return os << compute_samples::to_string(x);
}

std::istream &operator>>(std::istream &is, compute::usm_type &x) {
  std::string s;
  is >> s;
  if (s == "host") {
    x = compute::usm_type::host;
  } else if (s == "device") {
    x = compute::usm_type::device;
  } else if (s == "shared") {
    x = compute::usm_type::shared;
  } else if (s == "unknown") {
    x = compute::usm_type::unknown;
  } else {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}
} // namespace compute
} // namespace boost
