/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_INTEL_USM_HPP
#define BOOST_COMPUTE_INTEL_USM_HPP

#include <boost/compute/context.hpp>

#include <CL/cl_ext_intel.h>

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

template <typename T> void mem_free(const context &context, T *ptr) {
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
