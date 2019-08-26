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

#include "ocl_utils/unified_shared_memory.hpp"

namespace compute_samples {
std::string to_string(const compute::usm_type &x) {
  if (x == compute::usm_type::host) {
    return "host";
  } else if (x == compute::usm_type::device) {
    return "device";
  } else if (x == compute::usm_type::shared) {
    return "shared";
  } else {
    return "unknown";
  }
}
} // namespace compute_samples

namespace boost {
namespace compute {
std::ostream &operator<<(std::ostream &os, const compute::usm_type &x) {
  return os << compute_samples::to_string(x);
}

std::istream &operator>>(std::istream &is, compute::usm_type &x) {
  std::string s = "";
  is >> s;
  if (s == "host") {
    x = compute::usm_type::host;
  } else if (s == "device") {
    x = compute::usm_type::device;
  } else if (s == "shared") {
    x = compute::usm_type::shared;
  } else {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}
} // namespace compute
} // namespace boost
