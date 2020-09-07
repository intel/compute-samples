/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"
#include "test_harness/test_harness.hpp"

#include "boost/compute/intel/command_queue.hpp"

namespace compute = boost::compute;

namespace {

const std::vector<compute::usm_type> mem_advise_memory_types = {
    compute::usm_type::shared, compute::usm_type::unknown};

class clEnqueueMemAdviseINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes
    : public CopyKernelTest {};

HWTEST_P(
    clEnqueueMemAdviseINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    GivenCopyKernelAfterMemoryMigrationThenExpectedOutputIsReturned) {
  kernel_.set_arg_mem_ptr(0, destination_->get() + destination_offset_);
  kernel_.set_arg_mem_ptr(1, source_->get() + source_offset_);
  kernel_.set_arg(2, sizeof(uint8_t));

  compute::command_queue_intel queue(compute::system::default_queue());
  queue.enqueue_mem_advise(destination_->get() + destination_offset_,
                           region_count_ * sizeof(uint8_t), 0);
  queue.enqueue_mem_advise(source_->get() + source_offset_,
                           region_count_ * sizeof(uint8_t), 0);
  queue.enqueue_1d_range_kernel(kernel_, 0, region_count_, 0);
  queue.finish();

  const std::vector<uint8_t> actual =
      destination_->read(region_count_, destination_offset_);
  EXPECT_EQ(reference_, actual);
}

const std::vector<CopyKernelCombination> mem_advise_offsets = {
    {256, 256, 0, 0}, {256, 128, 128, 0}, {256, 128, 0, 128}};

INSTANTIATE_TEST_SUITE_P(
    Offsets,
    clEnqueueMemAdviseINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    ::testing::Combine(::testing::ValuesIn(mem_advise_offsets),
                       ::testing::ValuesIn(mem_advise_memory_types),
                       ::testing::ValuesIn(mem_advise_memory_types)),
    copy_kernel_test_name_suffix);

HWTEST(clGetEventInfo,
       GivenEventReturnedByMemAdviseThenCommandMemAdviseIsReturned) {
  const compute::usm_type memory_type = compute::usm_type::shared;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  const int count = 256;
  std::unique_ptr<MemoryWrapper<uint8_t>> source =
      get_memory_wrapper<uint8_t>(memory_type);
  source->allocate(count);

  compute::command_queue_intel queue(compute::system::default_queue());
  compute::event event =
      queue.enqueue_mem_advise(source->get(), count * sizeof(uint8_t), 0);
  queue.finish();

  EXPECT_EQ(CL_COMMAND_MEMADVISE_INTEL, event.get_command_type());

  source->free();
}

} // namespace
