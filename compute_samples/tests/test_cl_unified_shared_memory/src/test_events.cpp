/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"
#include "test_harness/test_harness.hpp"

#include "boost/compute/intel/command_queue.hpp"

namespace cs = compute_samples;
namespace compute = boost::compute;

namespace {

class WaitListEventTest : public ::testing::Test {
protected:
  void SetUp() override {
    if (!is_memory_access_supported(memory_type_)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(memory_type_));
    }

    source_ = get_memory_wrapper<uint8_t>(memory_type_);
    destination_ = get_memory_wrapper<uint8_t>(memory_type_);

    source_->allocate(allocation_count_);
    destination_->allocate(allocation_count_);

    std::vector<uint8_t> source_data(allocation_count_);
    std::iota(source_data.begin(), source_data.end(), 0u);
    source_->write(source_data);

    const std::vector<uint8_t> destination_data(allocation_count_, 0u);
    destination_->write(destination_data);

    compute::context context = compute::system::default_context();
    compute::program program = cs::build_program(
        context, "test_cl_unified_shared_memory.cl", "-cl-std=CL2.0");
    kernel_ = compute::kernel_intel(program.create_kernel("copy_kernel"));

    kernel_.set_arg_mem_ptr(0, destination_->get());
    kernel_.set_arg_mem_ptr(1, source_->get());
    kernel_.set_arg(2, sizeof(uint8_t));

    queue_ = compute::command_queue_intel(
        compute::system::default_context(), compute::system::default_device(),
        compute::command_queue::properties::enable_out_of_order_execution);
  }

  void TearDown() override {
    destination_->free();
    source_->free();
  }

  const int allocation_count_ = 256;
  compute::command_queue_intel queue_;
  compute::kernel_intel kernel_;
  std::unique_ptr<MemoryWrapper<uint8_t>> source_;
  std::unique_ptr<MemoryWrapper<uint8_t>> destination_;
  const compute::usm_type memory_type_ = compute::usm_type::device;
};

HWTEST_F(
    WaitListEventTest,
    GivenEventReturnedByMemcpyAsWaitListThenNDRangeKernelIsProperlyExecuted) {
  const std::vector<uint8_t> reference(allocation_count_, 32u);
  std::unique_ptr<MemoryWrapper<uint8_t>> tmp =
      get_memory_wrapper<uint8_t>(memory_type_);
  tmp->allocate(allocation_count_);
  tmp->write(reference);

  compute::event event = queue_.enqueue_memcpy_async(
      source_->get(), tmp->get(), allocation_count_ * sizeof(uint8_t));
  queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0,
                                 compute::wait_list(event));
  queue_.finish();

  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);

  tmp->free();
}

HWTEST_F(
    WaitListEventTest,
    GivenEventReturnedByMemFillAsWaitListThenNDRangeKernelIsProperlyExecuted) {
  const uint8_t pattern = 20;

  compute::event event =
      queue_.enqueue_mem_fill(source_->get(), &pattern, sizeof(pattern),
                              allocation_count_ * sizeof(uint8_t));
  queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0,
                                 compute::wait_list(event));
  queue_.finish();

  const std::vector<uint8_t> reference(allocation_count_, pattern);
  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

HWTEST_F(
    WaitListEventTest,
    GivenEventReturnedByMigrateMemAsWaitListThenNDRangeKernelIsProperlyExecuted) {
  const std::vector<uint8_t> reference = source_->read();

  compute::event event = queue_.enqueue_migrate_mem(
      source_->get(), allocation_count_ * sizeof(uint8_t), 0);
  queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0,
                                 compute::wait_list(event));
  queue_.finish();

  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

HWTEST_F(
    WaitListEventTest,
    GivenEventReturnedByMemAdviseAsWaitListThenNDRangeKernelIsProperlyExecuted) {
  const std::vector<uint8_t> reference = source_->read();

  compute::event event = queue_.enqueue_mem_advise(
      source_->get(), allocation_count_ * sizeof(uint8_t), 0);
  queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0,
                                 compute::wait_list(event));
  queue_.finish();

  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

HWTEST_F(
    WaitListEventTest,
    GivenEventReturnedByNDRangeKernelAsWaitListThenMemcpyIsProperlyExecuted) {
  const std::vector<uint8_t> reference(allocation_count_, 32u);
  std::unique_ptr<MemoryWrapper<uint8_t>> tmp =
      get_memory_wrapper<uint8_t>(memory_type_);
  tmp->allocate(allocation_count_);
  tmp->write(reference);

  compute::event event =
      queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0);
  queue_.enqueue_memcpy_async(destination_->get(), tmp->get(),
                              allocation_count_ * sizeof(uint8_t),
                              compute::wait_list(event));
  queue_.finish();

  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);

  tmp->free();
}

HWTEST_F(
    WaitListEventTest,
    GivenEventReturnedByNDRangeKernelAsWaitListThenMemFillIsProperlyExecuted) {
  const uint8_t pattern = 20;

  compute::event event =
      queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0);
  queue_.enqueue_mem_fill(destination_->get(), &pattern, sizeof(pattern),
                          allocation_count_ * sizeof(uint8_t),
                          compute::wait_list(event));
  queue_.finish();

  const std::vector<uint8_t> reference(allocation_count_, pattern);
  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

HWTEST_F(
    WaitListEventTest,
    GivenEventReturnedByNDRangeKernelAsWaitListThenMigrateMemIsProperlyExecuted) {
  const std::vector<uint8_t> reference = source_->read();

  compute::event event =
      queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0);
  queue_.enqueue_migrate_mem(source_->get(),
                             allocation_count_ * sizeof(uint8_t), 0,
                             compute::wait_list(event));
  queue_.finish();

  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

HWTEST_F(
    WaitListEventTest,
    GivenEventReturnedByNDRangeKernelAsWaitListThenMemAdviseIsProperlyExecuted) {
  const std::vector<uint8_t> reference = source_->read();

  compute::event event =
      queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0);
  queue_.enqueue_mem_advise(source_->get(), allocation_count_ * sizeof(uint8_t),
                            0, compute::wait_list(event));
  queue_.finish();

  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

} // namespace
