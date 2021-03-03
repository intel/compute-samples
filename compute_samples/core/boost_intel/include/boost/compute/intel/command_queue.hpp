/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_INTEL_COMMAND_QUEUE_HPP
#define BOOST_COMPUTE_INTEL_COMMAND_QUEUE_HPP

#include <boost/compute/command_queue.hpp>
#include <boost/compute/intel/device.hpp>

#include "ocl_entrypoints/cl_intel_aubs.h"
#include <CL/cl_ext.h>

namespace boost {
namespace compute {

class command_queue_intel : public command_queue {
  using command_queue::command_queue;

public:
  command_queue_intel() : command_queue() {}

  explicit command_queue_intel(const command_queue &other)
      : command_queue(other) {}

  command_queue_intel(const context &context, const device &device,
                      const cl_queue_properties *queue_properties)
      : command_queue(create_command_queue_with_properties(context, device,
                                                           queue_properties),
                      false) {}

  void enqueue_verify_memory(const void *actual_data, const void *expected_data,
                             size_t size_of_comparison,
                             cl_uint comparison_mode) {

    cl_int ret = clEnqueueVerifyMemoryINTEL(
        get(), actual_data, expected_data, size_of_comparison, comparison_mode);

    if (ret != CL_SUCCESS) {
      BOOST_THROW_EXCEPTION(opencl_error(ret));
    }
  }

  template <typename T>
  void enqueue_verify_memory(const void *actual_data,
                             const std::vector<T> &expected_data,
                             cl_uint comparison_mode) {
    enqueue_verify_memory(actual_data, expected_data.data(),
                          expected_data.size() * sizeof(T), comparison_mode);
  }

  event enqueue_mem_fill(void *dst_ptr, const void *pattern,
                         size_t pattern_size, size_t size,
                         const wait_list &events = wait_list()) {
    event event_;

    cl_int ret = clEnqueueMemFillINTEL(get(), dst_ptr, pattern, pattern_size,
                                       size, events.size(),
                                       events.get_event_ptr(), &event_.get());

    if (ret != CL_SUCCESS) {
      BOOST_THROW_EXCEPTION(opencl_error(ret));
    }

    return event_;
  }

  event enqueue_memcpy(void *dst_ptr, const void *src_ptr, size_t size,
                       const wait_list &events = wait_list()) {
    event event_;

    cl_int ret = clEnqueueMemcpyINTEL(get(), CL_TRUE, dst_ptr, src_ptr, size,
                                      events.size(), events.get_event_ptr(),
                                      &event_.get());

    if (ret != CL_SUCCESS) {
      BOOST_THROW_EXCEPTION(opencl_error(ret));
    }

    return event_;
  }

  event enqueue_memcpy_async(void *dst_ptr, const void *src_ptr, size_t size,
                             const wait_list &events = wait_list()) {
    event event_;

    cl_int ret = clEnqueueMemcpyINTEL(get(), CL_FALSE, dst_ptr, src_ptr, size,
                                      events.size(), events.get_event_ptr(),
                                      &event_.get());

    if (ret != CL_SUCCESS) {
      BOOST_THROW_EXCEPTION(opencl_error(ret));
    }

    return event_;
  }

  event enqueue_migrate_mem(const void *ptr, size_t size,
                            cl_mem_migration_flags flags,
                            const wait_list &events = wait_list()) {
    event event_;

    cl_int ret =
        clEnqueueMigrateMemINTEL(get(), ptr, size, flags, events.size(),
                                 events.get_event_ptr(), &event_.get());

    if (ret != CL_SUCCESS) {
      BOOST_THROW_EXCEPTION(opencl_error(ret));
    }

    return event_;
  }

  event enqueue_mem_advise(const void *ptr, size_t size,
                           cl_mem_advice_intel advice,
                           const wait_list &events = wait_list()) {
    event event_;

    cl_int ret =
        clEnqueueMemAdviseINTEL(get(), ptr, size, advice, events.size(),
                                events.get_event_ptr(), &event_.get());

    if (ret != CL_SUCCESS) {
      BOOST_THROW_EXCEPTION(opencl_error(ret));
    }

    return event_;
  }

  cl_uint family() const { return get_info<cl_uint>(CL_QUEUE_FAMILY_INTEL); }
  cl_uint index() const { return get_info<cl_uint>(CL_QUEUE_INDEX_INTEL); }

  cl_queue_family_properties_intel queue_family_properties() const {
    const device_intel device(get_device());
    const std::vector<cl_queue_family_properties_intel> families =
        device.queue_family_properties();
    return families[family()];
  }

  bool are_capabilities_supported(
      const cl_command_queue_capabilities_intel capabilities) const {
    const cl_queue_family_properties_intel properties =
        queue_family_properties();
    return (properties.capabilities == CL_QUEUE_DEFAULT_CAPABILITIES_INTEL) ||
           ((properties.capabilities & capabilities) == capabilities);
  }

  operator command_queue() const { return command_queue(get(), false); }

private:
  static cl_command_queue create_command_queue_with_properties(
      const context &context, const device &device,
      const cl_queue_properties *queue_properties) {
    cl_int error = CL_SUCCESS;
    cl_command_queue q = clCreateCommandQueueWithProperties(
        context, device.id(), queue_properties, &error);
    if (!q) {
      BOOST_THROW_EXCEPTION(opencl_error(error));
    }
    return q;
  }
};

} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_INTEL_COMMAND_QUEUE_HPP
