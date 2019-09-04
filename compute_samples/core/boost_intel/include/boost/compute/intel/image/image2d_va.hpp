/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_IMAGE_IMAGE2D_VA_HPP
#define BOOST_COMPUTE_IMAGE_IMAGE2D_VA_HPP

#if defined(__linux__)

#include <boost/throw_exception.hpp>

#include <boost/compute/config.hpp>
#include <boost/compute/context.hpp>
#include <boost/compute/exception/opencl_error.hpp>
#include <boost/compute/image/image2d.hpp>
#include <boost/compute/image/image_object.hpp>
#include <boost/compute/type_traits/type_name.hpp>

#include <CL/cl_va_api_media_sharing_intel.h>
#include <va/va.h>

namespace boost {
namespace compute {

// forward declarations
class command_queue;

/// \class image2d_va
/// \brief An OpenCL 2D image object for a VAAPI surface
///
/// See https://www.khronos.org/registry/OpenCL/extensions/intel/
/// cl_intel_va_api_media_sharing.txt
///
/// \snippet test/test_image2d_va.cpp create_image
///
/// \see image_format, image2d
class image2d_va : public image2d {
public:
  /// Creates a null image2d_va object.
  image2d_va() : image2d() {}

  /// Creates a new image2d_va object.
  ///
  /// \see_opencl_ref{clCreateImage}
  image2d_va(const context &context, VASurfaceID *va_surface, cl_uint plane,
             cl_mem_flags flags = read_write) {
    cl_int error = 0;

    clCreateFromVA_APIMediaSurfaceINTEL_fn cl_create_va_surface =
        (clCreateFromVA_APIMediaSurfaceINTEL_fn)context.get_device()
            .platform()
            .get_extension_function_address(
                "clCreateFromVA_APIMediaSurfaceINTEL");

    if (cl_create_va_surface) {
      m_mem =
          cl_create_va_surface(context.get(), flags, va_surface, plane, &error);
    } else {
      error = CL_OUT_OF_RESOURCES;
    }

    if (!m_mem) {
      BOOST_THROW_EXCEPTION(opencl_error(error));
    }
  }

  /// Creates a new image2d_va as a copy of \p other.
  image2d_va(const image2d_va &other) : image2d(other) {}

  /// Copies the image2d_va from \p other.
  image2d_va &operator=(const image2d_va &other) {
    image_object::operator=(other);
    return *this;
  }

#ifndef BOOST_COMPUTE_NO_RVALUE_REFERENCES
  /// Move-constructs a new image object from \p other.
  image2d_va(image2d_va &&other) BOOST_NOEXCEPT : image2d(std::move(other)) {}

  /// Move-assigns the image from \p other to \c *this.
  image2d_va &operator=(image2d_va &&other) BOOST_NOEXCEPT {
    image_object::operator=(std::move(other));

    return *this;
  }
#endif // BOOST_COMPUTE_NO_RVALUE_REFERENCES

  /// Destroys the image2d_va object.
  ~image2d_va() {}

  /// Creates a new image with a copy of the data in \c *this. Uses \p queue
  /// to perform the copy operation.
  image2d_va clone(command_queue &queue) const;
};

namespace detail {

// set_kernel_arg() specialization for image2d_va
template <>
struct set_kernel_arg<image2d_va> : public set_kernel_arg<image_object> {};

} // namespace detail
} // namespace compute
} // namespace boost

BOOST_COMPUTE_TYPE_NAME(boost::compute::image2d_va, image2d_va_t)

#endif // __linux__

#endif // BOOST_COMPUTE_IMAGE_IMAGE2D_VA_HPP
