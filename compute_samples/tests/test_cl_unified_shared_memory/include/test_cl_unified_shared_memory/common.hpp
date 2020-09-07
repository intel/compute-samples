/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#ifndef COMPUTE_SAMPLES_TEST_CL_UNIFIED_SHARED_MEMORY_COMMON_HPP
#define COMPUTE_SAMPLES_TEST_CL_UNIFIED_SHARED_MEMORY_COMMON_HPP

#include <string>
#include <sstream>
#include <numeric>

#include "gtest/gtest.h"

#include "boost/compute/intel/usm.hpp"
#include "boost/compute/intel/kernel.hpp"

#include "utils/utils.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "ocl_utils/unified_shared_memory.hpp"
#include "test_harness/test_harness.hpp"

namespace cs = compute_samples;

bool is_memory_access_supported(const boost::compute::usm_type t);
bool is_atomic_access_supported(const boost::compute::usm_type t);

template <typename T> class MemoryWrapper {
public:
  virtual ~MemoryWrapper() = default;

  virtual void
  allocate(const int count,
           const std::vector<cl_mem_properties_intel> properties = {0}) = 0;
  virtual void free() = 0;

  virtual void write(const std::vector<T> &source, const int offset = 0) = 0;
  virtual std::vector<T> read(const int count = 0, const int offset = 0) = 0;

  virtual T *get() = 0;
};

template <typename T> class HostMemoryWrapper : public MemoryWrapper<T> {
public:
  HostMemoryWrapper(const boost::compute::context &context)
      : context_(context) {}

  void allocate(const int count,
                const std::vector<cl_mem_properties_intel> properties = {
                    0}) override {
    count_ = static_cast<size_t>(count);
    data_ = boost::compute::host_mem_alloc<T>(context_, properties.data(),
                                              count_, 0);
  }

  void free() override {
    boost::compute::mem_free(context_, data_);
    count_ = 0;
  }

  void write(const std::vector<T> &source, const int offset = 0) override {
    std::copy(source.begin(), source.end(), data_ + offset);
  }

  std::vector<T> read(const int count = 0, const int offset = 0) override {
    size_t c = count ? static_cast<size_t>(count) : count_;
    return std::vector<T>(data_ + offset, data_ + offset + c);
  }

  T *get() override { return data_; }

private:
  boost::compute::context context_;
  T *data_ = nullptr;
  size_t count_;
};

template <typename T> class DeviceMemoryWrapper : public MemoryWrapper<T> {
public:
  DeviceMemoryWrapper(const boost::compute::context &context,
                      const boost::compute::device &device)
      : context_(context), device_(device) {}

  void allocate(const int count,
                const std::vector<cl_mem_properties_intel> properties = {
                    0}) override {
    count_ = static_cast<size_t>(count);
    data_ = boost::compute::device_mem_alloc<T>(context_, device_,
                                                properties.data(), count_, 0);
  }

  void free() override {
    boost::compute::mem_free(context_, data_);
    count_ = 0;
  }

  void write(const std::vector<T> &source, const int offset = 0) override {
    T *p = const_cast<T *>(source.data());
    boost::compute::buffer buffer(context_,
                                  compute_samples::size_in_bytes(source),
                                  boost::compute::buffer::read_only |
                                      boost::compute::buffer::copy_host_ptr,
                                  p);

    boost::compute::program program = compute_samples::build_program(
        context_, "test_cl_unified_shared_memory.cl", "-cl-std=CL2.0");
    boost::compute::kernel_intel kernel(program.create_kernel("copy_kernel"));

    kernel.set_arg_mem_ptr(0, data_ + offset);
    kernel.set_arg(1, buffer);
    kernel.set_arg(2, sizeof(T));

    boost::compute::command_queue queue =
        boost::compute::system::default_queue();
    queue.enqueue_1d_range_kernel(kernel, 0, source.size(), 0);
    queue.finish();
  }

  std::vector<T> read(const int count = 0, const int offset = 0) override {
    size_t c = count ? static_cast<size_t>(count) : count_;
    std::vector<T> destination(c);

    boost::compute::buffer buffer(context_,
                                  compute_samples::size_in_bytes(destination),
                                  boost::compute::buffer::write_only);

    boost::compute::program program = compute_samples::build_program(
        context_, "test_cl_unified_shared_memory.cl", "-cl-std=CL2.0");
    boost::compute::kernel_intel kernel(program.create_kernel("copy_kernel"));

    kernel.set_arg(0, buffer);
    kernel.set_arg_mem_ptr(1, data_ + offset);
    kernel.set_arg(2, sizeof(T));

    boost::compute::command_queue queue =
        boost::compute::system::default_queue();
    queue.enqueue_1d_range_kernel(kernel, 0, destination.size(), 0);
    queue.enqueue_read_buffer_async(buffer, 0,
                                    compute_samples::size_in_bytes(destination),
                                    destination.data());
    queue.finish();

    return destination;
  }

  T *get() override { return data_; }

private:
  boost::compute::context context_;
  boost::compute::device device_;
  T *data_ = nullptr;
  size_t count_;
};

template <typename T> class SharedMemoryWrapper : public MemoryWrapper<T> {
public:
  SharedMemoryWrapper(const boost::compute::context &context,
                      const boost::compute::device &device)
      : context_(context), device_(device) {}

  void allocate(const int count,
                const std::vector<cl_mem_properties_intel> properties = {
                    0}) override {
    count_ = static_cast<size_t>(count);
    data_ = boost::compute::shared_mem_alloc<T>(context_, device_,
                                                properties.data(), count_, 0);
  }

  void free() override {
    boost::compute::mem_free(context_, data_);
    count_ = 0;
  }

  void write(const std::vector<T> &source, const int offset = 0) override {
    std::copy(source.begin(), source.end(), data_ + offset);
  }

  std::vector<T> read(const int count = 0, const int offset = 0) override {
    size_t c = count ? static_cast<size_t>(count) : count_;
    return std::vector<T>(data_ + offset, data_ + offset + c);
  }

  T *get() override { return data_; }

private:
  boost::compute::context context_;
  boost::compute::device device_;
  T *data_ = nullptr;
  size_t count_;
};

template <typename T> class SystemMemoryWrapper : public MemoryWrapper<T> {
public:
  void allocate(const int count,
                const std::vector<cl_mem_properties_intel> properties = {
                    0}) override {
    count_ = static_cast<size_t>(count);
    data_ = static_cast<T *>(std::malloc(count_ * sizeof(T)));
  }

  void free() override {
    std::free(data_);
    count_ = 0;
  }

  void write(const std::vector<T> &source, const int offset = 0) override {
    std::copy(source.begin(), source.end(), data_ + offset);
  }

  std::vector<T> read(const int count = 0, const int offset = 0) override {
    size_t c = count ? static_cast<size_t>(count) : count_;
    return std::vector<T>(data_ + offset, data_ + offset + c);
  }

  T *get() override { return data_; }

private:
  T *data_ = nullptr;
  size_t count_;
};

template <typename T>
std::unique_ptr<MemoryWrapper<T>>
get_memory_wrapper(const boost::compute::usm_type type) {
  if (type == boost::compute::usm_type::host) {
    return std::unique_ptr<MemoryWrapper<T>>(
        new HostMemoryWrapper<T>(boost::compute::system::default_context()));
  } else if (type == boost::compute::usm_type::device) {
    return std::unique_ptr<MemoryWrapper<T>>(
        new DeviceMemoryWrapper<T>(boost::compute::system::default_context(),
                                   boost::compute::system::default_device()));
  } else if (type == boost::compute::usm_type::shared) {
    return std::unique_ptr<MemoryWrapper<T>>(
        new SharedMemoryWrapper<T>(boost::compute::system::default_context(),
                                   boost::compute::system::default_device()));
  } else {
    return std::unique_ptr<MemoryWrapper<T>>(new SystemMemoryWrapper<T>());
  }
}

struct CopyKernelCombination {
  int allocation_count;
  int region_count;
  int source_offset;
  int destination_offset;
};

std::string to_string(const CopyKernelCombination &x);
std::ostream &operator<<(std::ostream &os, const CopyKernelCombination &x);

class CopyKernelTest
    : public ::testing::TestWithParam<
          std::tuple<CopyKernelCombination, boost::compute::usm_type,
                     boost::compute::usm_type>> {
protected:
  void SetUp() override {
    const CopyKernelCombination params = std::get<0>(GetParam());
    region_count_ = params.region_count;
    source_offset_ = params.source_offset;
    destination_offset_ = params.destination_offset;

    const boost::compute::usm_type source_type = std::get<1>(GetParam());
    source_ = get_memory_wrapper<uint8_t>(source_type);

    const boost::compute::usm_type destination_type = std::get<2>(GetParam());
    destination_ = get_memory_wrapper<uint8_t>(destination_type);

    if (!is_memory_access_supported(source_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(source_type));
    }
    if (!is_memory_access_supported(destination_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(destination_type));
    }

    const int allocation_count = params.allocation_count;
    source_->allocate(allocation_count);
    destination_->allocate(allocation_count);

    std::vector<uint8_t> source_data(allocation_count);
    std::iota(source_data.begin(), source_data.end(), 0u);
    source_->write(source_data);

    const std::vector<uint8_t> destination_data(region_count_, 0u);
    destination_->write(destination_data);

    reference_ = std::vector<uint8_t>(source_data.begin() + source_offset_,
                                      source_data.begin() + source_offset_ +
                                          region_count_);

    boost::compute::context context = boost::compute::system::default_context();
    boost::compute::program program = compute_samples::build_program(
        context, "test_cl_unified_shared_memory.cl", "-cl-std=CL2.0");
    kernel_ =
        boost::compute::kernel_intel(program.create_kernel("copy_kernel"));
  }

  void TearDown() override {
    destination_->free();
    source_->free();
  }

  int region_count_;
  int source_offset_;
  int destination_offset_;
  std::vector<uint8_t> reference_;
  std::unique_ptr<MemoryWrapper<uint8_t>> source_;
  std::unique_ptr<MemoryWrapper<uint8_t>> destination_;
  boost::compute::kernel_intel kernel_;
};

std::string copy_kernel_test_name_suffix(
    testing::TestParamInfo<
        std::tuple<CopyKernelCombination, boost::compute::usm_type,
                   boost::compute::usm_type>>
        param_info);

#endif
