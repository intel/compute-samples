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

class clEnqueueMemcpyINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes
    : public CopyKernelTest {};

HWTEST_P(
    clEnqueueMemcpyINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    GivenBlockingCallThenExpectedOutputIsReturned) {
  compute::command_queue_intel queue(compute::system::default_queue());
  queue.enqueue_memcpy(destination_->get() + destination_offset_,
                       source_->get() + source_offset_,
                       region_count_ * sizeof(uint8_t));

  const std::vector<uint8_t> actual =
      destination_->read(region_count_, destination_offset_);
  EXPECT_EQ(reference_, actual);
}

HWTEST_P(
    clEnqueueMemcpyINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    GivenNonBlockingCallThenExpectedOutputIsReturned) {
  compute::command_queue_intel queue(compute::system::default_queue());
  queue.enqueue_memcpy_async(destination_->get() + destination_offset_,
                             source_->get() + source_offset_,
                             region_count_ * sizeof(uint8_t));
  queue.finish();

  const std::vector<uint8_t> actual =
      destination_->read(region_count_, destination_offset_);
  EXPECT_EQ(reference_, actual);
}

const std::vector<compute::usm_type> memcpy_memory_types = {
    compute::usm_type::host, compute::usm_type::device,
    compute::usm_type::shared, compute::usm_type::unknown};

const std::vector<CopyKernelCombination> memcpy_allocation_count = {
    {1, 1, 0, 0},      {2, 2, 0, 0},     {4, 4, 0, 0},       {8, 8, 0, 0},
    {16, 16, 0, 0},    {32, 32, 0, 0},   {64, 64, 0, 0},     {128, 128, 0, 0},
    {256, 256, 0, 0},  {512, 512, 0, 0}, {1024, 1024, 0, 0}, {2048, 2048, 0, 0},
    {4096, 4096, 0, 0}};

INSTANTIATE_TEST_SUITE_P(
    AllocationCount,
    clEnqueueMemcpyINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    ::testing::Combine(::testing::ValuesIn(memcpy_allocation_count),
                       ::testing::ValuesIn(memcpy_memory_types),
                       ::testing::ValuesIn(memcpy_memory_types)),
    copy_kernel_test_name_suffix);

const std::vector<CopyKernelCombination> memcpy_region_count = {
    {2, 1, 0, 0},      {4, 2, 0, 0},       {8, 4, 0, 0},
    {16, 8, 0, 0},     {32, 16, 0, 0},     {64, 32, 0, 0},
    {128, 64, 0, 0},   {256, 128, 0, 0},   {512, 256, 0, 0},
    {1024, 512, 0, 0}, {2048, 1024, 0, 0}, {4096, 2048, 0, 0}};

INSTANTIATE_TEST_SUITE_P(
    RegionCount,
    clEnqueueMemcpyINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    ::testing::Combine(::testing::ValuesIn(memcpy_region_count),
                       ::testing::ValuesIn(memcpy_memory_types),
                       ::testing::ValuesIn(memcpy_memory_types)),
    copy_kernel_test_name_suffix);

const std::vector<CopyKernelCombination> memcpy_offsets = {
    {2, 1, 1, 0},          {2, 1, 0, 1},          {4, 2, 2, 0},
    {4, 2, 0, 2},          {8, 4, 4, 0},          {8, 4, 0, 4},
    {16, 8, 8, 0},         {16, 8, 0, 8},         {32, 16, 16, 0},
    {32, 16, 0, 16},       {64, 32, 32, 0},       {64, 32, 0, 32},
    {128, 64, 64, 0},      {128, 64, 0, 64},      {256, 128, 128, 0},
    {256, 128, 0, 128},    {512, 256, 256, 0},    {512, 256, 0, 256},
    {1024, 512, 512, 0},   {1024, 512, 0, 512},   {2048, 1024, 1024, 0},
    {2048, 1024, 0, 1024}, {4096, 2048, 2048, 0}, {4096, 2048, 0, 2048}};

INSTANTIATE_TEST_SUITE_P(
    Offsets,
    clEnqueueMemcpyINTELWithAllocationCountRegionCountOffsetsAndMemoryTypes,
    ::testing::Combine(::testing::ValuesIn(memcpy_offsets),
                       ::testing::ValuesIn(memcpy_memory_types),
                       ::testing::ValuesIn(memcpy_memory_types)),
    copy_kernel_test_name_suffix);

HWTEST(clGetEventInfo, GivenEventReturnedByMemcpyThenCommandMemcpyIsReturned) {
  const compute::usm_type memory_type = compute::usm_type::device;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  const int count = 256;
  std::unique_ptr<MemoryWrapper<uint8_t>> source =
      get_memory_wrapper<uint8_t>(memory_type);
  source->allocate(count);

  std::unique_ptr<MemoryWrapper<uint8_t>> destination =
      get_memory_wrapper<uint8_t>(memory_type);
  destination->allocate(count);

  compute::command_queue_intel queue(compute::system::default_queue());
  compute::event event = queue.enqueue_memcpy(destination->get(), source->get(),
                                              count * sizeof(uint8_t));
  queue.finish();

  EXPECT_EQ(CL_COMMAND_MEMCPY_INTEL, event.get_command_type());

  source->free();
  destination->free();
}

} // namespace
