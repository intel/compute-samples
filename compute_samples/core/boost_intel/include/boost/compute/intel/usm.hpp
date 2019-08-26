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

#ifndef BOOST_COMPUTE_INTEL_USM_HPP
#define BOOST_COMPUTE_INTEL_USM_HPP

#include <boost/compute/context.hpp>

#include "ocl_entrypoints/cl_intel_unified_shared_memory.h"

namespace boost {
namespace compute {

enum class usm_type {
  host = CL_MEM_TYPE_HOST_INTEL,
  device = CL_MEM_TYPE_DEVICE_INTEL,
  shared = CL_MEM_TYPE_SHARED_INTEL,
  unknown = CL_MEM_TYPE_UNKNOWN_INTEL
};

template <typename T>
T *host_mem_alloc(const context &context,
                  const cl_mem_properties_intel *properties, size_t size,
                  cl_uint alignment = 0) {
  cl_int error = CL_SUCCESS;
  T *ptr = static_cast<T *>(clHostMemAllocINTEL(
      context.get(), properties, size * sizeof(T), alignment, &error));
  if (!ptr) {
    BOOST_THROW_EXCEPTION(opencl_error(error));
  }
  return ptr;
}

template <typename T>
T *device_mem_alloc(const context &context, const device &device,
                    const cl_mem_properties_intel *properties, size_t size,
                    cl_uint alignment = 0) {
  cl_int error = CL_SUCCESS;
  T *ptr = static_cast<T *>(clDeviceMemAllocINTEL(context.get(), device.get(),
                                                  properties, size * sizeof(T),
                                                  alignment, &error));
  if (!ptr) {
    BOOST_THROW_EXCEPTION(opencl_error(error));
  }
  return ptr;
}

template <typename T>
T *shared_mem_alloc(const context &context, const device &device,
                    const cl_mem_properties_intel *properties, size_t size,
                    cl_uint alignment = 0) {
  cl_int error = CL_SUCCESS;
  T *ptr = static_cast<T *>(clSharedMemAllocINTEL(context.get(), device.get(),
                                                  properties, size * sizeof(T),
                                                  alignment, &error));
  if (!ptr) {
    BOOST_THROW_EXCEPTION(opencl_error(error));
  }
  return ptr;
}

template <typename T> void mem_free(const context &context, const T *ptr) {
  cl_int error = clMemFreeINTEL(context.get(), ptr);
  if (error != CL_SUCCESS) {
    BOOST_THROW_EXCEPTION(opencl_error(error));
  }
}

template <typename T, typename U>
T get_mem_alloc_info(const context &context, const U *ptr,
                     cl_mem_info_intel info) {
  T value;
  cl_int error = clGetMemAllocInfoINTEL(context.get(), ptr, info, sizeof(T),
                                        &value, nullptr);
  if (error != CL_SUCCESS) {
    BOOST_THROW_EXCEPTION(opencl_error(error));
  }
  return value;
}

} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_INTEL_USM_HPP
