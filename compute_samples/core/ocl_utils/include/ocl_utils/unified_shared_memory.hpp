/*
 * Copyright(c) 2019 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef COMPUTE_SAMPLES_OCL_UTILS_UNIFIED_SHARED_MEMORY_HPP
#define COMPUTE_SAMPLES_OCL_UTILS_UNIFIED_SHARED_MEMORY_HPP

#include <iostream>
#include <string>

#include "boost/compute/intel/usm.hpp"
namespace compute = boost::compute;

namespace compute_samples {
std::string to_string(const compute::usm_type &x);
} // namespace compute_samples

namespace boost {
namespace compute {
std::ostream &operator<<(std::ostream &os, const compute::usm_type &x);
std::istream &operator>>(std::istream &is, compute::usm_type &x);
} // namespace compute
} // namespace boost

#endif
