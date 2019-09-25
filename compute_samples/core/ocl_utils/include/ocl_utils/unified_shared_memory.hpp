/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_OCL_UTILS_UNIFIED_SHARED_MEMORY_HPP
#define COMPUTE_SAMPLES_OCL_UTILS_UNIFIED_SHARED_MEMORY_HPP

#include <iostream>
#include <string>

#include "boost/compute/intel/usm.hpp"

namespace compute_samples {
std::string to_string(const boost::compute::usm_type &x);
} // namespace compute_samples

namespace boost {
namespace compute {
std::ostream &operator<<(std::ostream &os, const boost::compute::usm_type &x);
std::istream &operator>>(std::istream &is, boost::compute::usm_type &x);
} // namespace compute
} // namespace boost

#endif
