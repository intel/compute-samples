/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_INTEL_AUBS_HPP
#define BOOST_COMPUTE_INTEL_AUBS_HPP

#include <boost/compute/platform.hpp>

#include "ocl_entrypoints/cl_intel_aubs.h"

namespace boost {
namespace compute {

void add_aub_comment(const platform &platform, const char *comment) {
  cl_int ret = clAddCommentINTEL(platform.id(), comment);

  if (ret != CL_SUCCESS) {
    BOOST_THROW_EXCEPTION(opencl_error(ret));
  }
}

void add_aub_comment(const platform &platform, const std::string comment) {
  add_aub_comment(platform, comment.c_str());
}

} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_INTEL_AUBS_HPP
