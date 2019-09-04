/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_IMAGE_IMAGE2D_INTEL_HPP
#define BOOST_COMPUTE_IMAGE_IMAGE2D_INTEL_HPP

#include <boost/compute/image/image_object.hpp>

namespace boost {
namespace compute {

class image2d_intel : public image_object {
  using image_object::image_object;

public:
  image2d_intel() : image_object() {}

  image2d_intel(const context &context, const cl_image_desc &desc,
                const cl_image_format &format, cl_mem_flags flags = read_write,
                void *host_ptr = 0)
      : image_object(create_image(context, flags, format, desc, host_ptr),
                     false) {}

private:
  static cl_mem create_image(const context &context, cl_mem_flags flags,
                             const cl_image_format &format,
                             const cl_image_desc &desc, void *host_ptr) {
    cl_int error = CL_SUCCESS;
    cl_mem m = clCreateImage(context, flags, &format, &desc, host_ptr, &error);
    if (!m) {
      BOOST_THROW_EXCEPTION(opencl_error(error));
    }
    return m;
  }
};

namespace detail {

template <>
struct set_kernel_arg<image2d_intel> : public set_kernel_arg<image_object> {};

} // namespace detail
} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_IMAGE_IMAGE2D_INTEL_HPP
