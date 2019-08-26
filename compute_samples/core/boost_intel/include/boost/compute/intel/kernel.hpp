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

#ifndef BOOST_COMPUTE_INTEL_KERNEL_HPP
#define BOOST_COMPUTE_INTEL_KERNEL_HPP

#include <boost/compute/kernel.hpp>

#include "ocl_entrypoints/cl_intel_unified_shared_memory.h"

namespace boost {
namespace compute {

class kernel_intel : public kernel {
  using kernel::kernel;

public:
  kernel_intel() : kernel() {}
  explicit kernel_intel(const kernel &other) : kernel(other) {}

  void set_arg_mem_ptr(size_t index, void *ptr) {
    cl_int ret =
        clSetKernelArgMemPointerINTEL(get(), static_cast<cl_uint>(index), ptr);
    if (ret != CL_SUCCESS) {
      BOOST_THROW_EXCEPTION(opencl_error(ret));
    }
  }

  operator kernel() const { return kernel(get(), false); }
};

} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_INTEL_KERNEL_HPP
