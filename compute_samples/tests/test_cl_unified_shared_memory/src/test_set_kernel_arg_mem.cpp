/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"
#include "test_harness/test_harness.hpp"

namespace compute = boost::compute;

namespace {

const std::vector<compute::usm_type> set_kernel_arg_memory_types = {
    compute::usm_type::host, compute::usm_type::device,
    compute::usm_type::shared, compute::usm_type::unknown};

class
    clSetKernelArgMemPointerINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes
    : public CopyKernelTest {};

HWTEST_P(
    clSetKernelArgMemPointerINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    GivenCopyKernelThenExpectedOutputIsReturned) {
  kernel_.set_arg_mem_ptr(0, destination_->get() + destination_offset_);
  kernel_.set_arg_mem_ptr(1, source_->get() + source_offset_);
  kernel_.set_arg(2, sizeof(uint8_t));

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel_, 0, region_count_, 0);
  queue.finish();

  const std::vector<uint8_t> actual =
      destination_->read(region_count_, destination_offset_);
  EXPECT_EQ(reference_, actual);
}

const std::vector<CopyKernelCombination> set_kernel_arg_offsets = {
    {256, 256, 0, 0}, {256, 128, 128, 0}, {256, 128, 0, 128}};

INSTANTIATE_TEST_SUITE_P(
    Offsets,
    clSetKernelArgMemPointerINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    ::testing::Combine(::testing::ValuesIn(set_kernel_arg_offsets),
                       ::testing::ValuesIn(set_kernel_arg_memory_types),
                       ::testing::ValuesIn(set_kernel_arg_memory_types)),
    copy_kernel_test_name_suffix);

} // namespace
