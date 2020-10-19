/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_INTEL_KERNEL_HPP
#define BOOST_COMPUTE_INTEL_KERNEL_HPP

#include <boost/compute/kernel.hpp>

#include <boost/compute/device.hpp>
#include <CL/cl_ext_intel.h>

namespace boost {
namespace compute {

class kernel_intel : public kernel {
  using kernel::kernel;

public:
  kernel_intel() : kernel() {}
  explicit kernel_intel(const kernel &other) : kernel(other) {}

  const cl_ulong private_mem_size(const device &device) const {
    return get_work_group_info<cl_ulong>(device, CL_KERNEL_PRIVATE_MEM_SIZE);
  }

  const cl_ulong spill_mem_size(const device &device) const {
    return get_work_group_info<cl_ulong>(device,
                                         CL_KERNEL_SPILL_MEM_SIZE_INTEL);
  }

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
