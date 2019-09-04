/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_IMAGE_IMAGE2D_YPLANE_HPP
#define BOOST_COMPUTE_IMAGE_IMAGE2D_YPLANE_HPP

#include <boost/throw_exception.hpp>

#include <boost/compute/config.hpp>
#include <boost/compute/context.hpp>
#include <boost/compute/exception/opencl_error.hpp>
#include <boost/compute/image/image2d.hpp>
#include <boost/compute/image/image_format.hpp>
#include <boost/compute/image/image_object.hpp>
#include <boost/compute/type_traits/type_name.hpp>

namespace boost {
namespace compute {

// forward declarations
class command_queue;

/// \class image2d_yplane
/// \brief An OpenCL 2D y-plane image object
///
/// See https://www.khronos.org/registry/OpenCL/extensions/intel/
/// cl_intel_planar_yuv.txt.
///
/// \snippet test/test_image2d_yplane.cpp create_image
///
/// \see image_format, image2d
class image2d_yplane : public image2d {
public:
  /// Creates a null image2d_yplane object.
  image2d_yplane() : image2d() {}

  /// Creates a new image2d_yplane object.
  ///
  /// \see_opencl_ref{clCreateImage}
  image2d_yplane(const context &context, image2d parent_nv12_image,
                 cl_mem_flags flags = read_write) {
    cl_int error = 0;
    image_format format(CL_R, CL_UNORM_INT8);

#ifdef CL_VERSION_1_2
    cl_image_desc desc;
    desc.image_type = CL_MEM_OBJECT_IMAGE2D;
    desc.image_width = 0;       // unused
    desc.image_height = 0;      // unused
    desc.image_depth = 0;       // set to y-plane index
    desc.image_array_size = 0;  // unused
    desc.image_row_pitch = 0;   // unused
    desc.image_slice_pitch = 0; // unused
    desc.num_mip_levels = 0;    // unused
    desc.num_samples = 0;       // unused
#ifdef CL_VERSION_2_0
    desc.mem_object = parent_nv12_image.get();
#else
    desc.buffer = parent_nv12_image.get();
#endif
    m_mem = clCreateImage(context, flags, format.get_format_ptr(), &desc, NULL,
                          &error);
#else
    m_mem = NULL;
#endif

    if (!m_mem) {
      BOOST_THROW_EXCEPTION(opencl_error(error));
    }
  }

  /// Creates a new image2d_yplane as a copy of \p other.
  image2d_yplane(const image2d_yplane &other) : image2d(other) {}

  /// Copies the image2d_yplane from \p other.
  image2d_yplane &operator=(const image2d_yplane &other) {
    image_object::operator=(other);
    return *this;
  }

#ifndef BOOST_COMPUTE_NO_RVALUE_REFERENCES
  /// Move-constructs a new image object from \p other.
  image2d_yplane(image2d_yplane &&other) BOOST_NOEXCEPT
      : image2d(std::move(other)) {}

  /// Move-assigns the image from \p other to \c *this.
  image2d_yplane &operator=(image2d_yplane &&other) BOOST_NOEXCEPT {
    image_object::operator=(std::move(other));

    return *this;
  }
#endif // BOOST_COMPUTE_NO_RVALUE_REFERENCES

  /// Destroys the image2d_yplane object.
  ~image2d_yplane() {}

  /// Creates a new image with a copy of the data in \c *this. Uses \p queue
  /// to perform the copy operation.
  image2d_yplane clone(command_queue &queue) const;
};

namespace detail {

// set_kernel_arg() specialization for image2d_yplane
template <>
struct set_kernel_arg<image2d_yplane> : public set_kernel_arg<image_object> {};

} // namespace detail
} // namespace compute
} // namespace boost

BOOST_COMPUTE_TYPE_NAME(boost::compute::image2d_yplane, image2d_yplane_t)

#endif // BOOST_COMPUTE_IMAGE_IMAGE2D_YPLANE_HPP
