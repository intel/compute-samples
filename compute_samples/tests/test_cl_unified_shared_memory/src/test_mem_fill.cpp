/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"
#include "test_harness/test_harness.hpp"

#include "boost/compute/intel/command_queue.hpp"
#include "boost/compute/intel/device.hpp"

namespace compute = boost::compute;

namespace {

const std::vector<compute::usm_type> mem_fill_memory_types = {
    compute::usm_type::host, compute::usm_type::device,
    compute::usm_type::shared, compute::usm_type::unknown};

struct MemFillCombination {
  int allocation_size;
  int region_size;
  int offset;
};

std::string to_string(const MemFillCombination &x) {
  std::stringstream ss;
  ss << "{";
  ss << "allocation_size: " << x.allocation_size << ", ";
  ss << "region_size: " << x.region_size << ", ";
  ss << "offset: " << x.offset;
  ss << "}";
  return ss.str();
}

std::ostream &operator<<(std::ostream &os, const MemFillCombination &x) {
  os << to_string(x);
  return os;
}

class clEnqueueMemFillINTELWithAllocationSizesRegionSizesOffsetsAndValues
    : public ::testing::TestWithParam<
          std::tuple<MemFillCombination, compute::usm_type>> {
protected:
  void SetUp() override {
    const MemFillCombination params = std::get<0>(GetParam());
    const int allocation_size = params.allocation_size;
    offset_ = params.offset;
    region_size_ = params.region_size;

    const compute::usm_type source_type = std::get<1>(GetParam());
    source_ = get_memory_wrapper<uint8_t>(source_type);

    if (!is_memory_access_supported(source_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(source_type));
    }

    source_->allocate(allocation_size);

    std::vector<uint8_t> source_data(allocation_size);
    std::iota(source_data.begin(), source_data.end(), 0u);
    source_->write(source_data);

    source_offset_ = source_->get() + offset_;

    pattern_ = 0x12;
    reference_ =
        std::vector<uint8_t>(static_cast<size_t>(region_size_), pattern_);
  }

  void TearDown() override { source_->free(); }

  uint8_t pattern_;
  int region_size_;
  int offset_;
  std::vector<uint8_t> reference_;
  std::unique_ptr<MemoryWrapper<uint8_t>> source_;
  uint8_t *source_offset_;
};

HWTEST_P(clEnqueueMemFillINTELWithAllocationSizesRegionSizesOffsetsAndValues,
         GivenMemFillThenExpectedOutputIsReturned) {
  compute::command_queue_intel queue(compute::system::default_queue());
  queue.enqueue_mem_fill(source_offset_, &pattern_, sizeof(pattern_),
                         static_cast<size_t>(region_size_));
  queue.finish();

  const std::vector<uint8_t> actual = source_->read(region_size_, offset_);

  EXPECT_EQ(reference_, actual);
}

std::string mem_fill_test_name_suffix(
    testing::TestParamInfo<std::tuple<MemFillCombination, compute::usm_type>>
        param_info) {
  const MemFillCombination m = std::get<0>(param_info.param);
  const compute::usm_type source = std::get<1>(param_info.param);

  std::stringstream ss;
  ss << "allocation_size_" << m.allocation_size << "_";
  ss << "region_size_" << m.region_size << "_";
  ss << "offset_" << m.offset << "_";
  ss << "source_" << source;
  return ss.str();
}

const std::vector<MemFillCombination> mem_fill_allocation_sizes = {
    {1, 1, 0},      {2, 2, 0},     {4, 4, 0},       {8, 8, 0},
    {16, 16, 0},    {32, 32, 0},   {64, 64, 0},     {128, 128, 0},
    {256, 256, 0},  {512, 512, 0}, {1024, 1024, 0}, {2048, 2048, 0},
    {4096, 4096, 0}};

INSTANTIATE_TEST_SUITE_P(
    AllocationSizes,
    clEnqueueMemFillINTELWithAllocationSizesRegionSizesOffsetsAndValues,
    ::testing::Combine(::testing::ValuesIn(mem_fill_allocation_sizes),
                       ::testing::ValuesIn(mem_fill_memory_types)),
    mem_fill_test_name_suffix);

const std::vector<MemFillCombination> mem_fill_region_sizes = {
    {2, 1, 0},     {4, 2, 0},      {8, 4, 0},       {16, 8, 0},
    {32, 16, 0},   {64, 32, 0},    {128, 64, 0},    {256, 128, 0},
    {512, 256, 0}, {1024, 512, 0}, {2048, 1024, 0}, {4096, 2048, 0}};

INSTANTIATE_TEST_SUITE_P(
    RegionSizes,
    clEnqueueMemFillINTELWithAllocationSizesRegionSizesOffsetsAndValues,
    ::testing::Combine(::testing::ValuesIn(mem_fill_region_sizes),
                       ::testing::ValuesIn(mem_fill_memory_types)),
    mem_fill_test_name_suffix);

const std::vector<MemFillCombination> mem_fill_offsets = {
    {2, 1, 1},       {4, 2, 2},       {8, 4, 4},     {16, 8, 8},
    {32, 16, 16},    {64, 32, 32},    {128, 64, 64}, {256, 128, 128},
    {512, 256, 256}, {1024, 512, 512}};

INSTANTIATE_TEST_SUITE_P(
    Offsets,
    clEnqueueMemFillINTELWithAllocationSizesRegionSizesOffsetsAndValues,
    ::testing::Combine(::testing::ValuesIn(mem_fill_offsets),
                       ::testing::ValuesIn(mem_fill_memory_types)),
    mem_fill_test_name_suffix);

HWTEST(clGetEventInfo,
       GivenEventReturnedByMemFillThenCommandMemFillIsReturned) {
  const compute::usm_type memory_type = compute::usm_type::device;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  const int count = 256;
  std::unique_ptr<MemoryWrapper<uint8_t>> source =
      get_memory_wrapper<uint8_t>(memory_type);
  source->allocate(count);

  compute::command_queue_intel queue(compute::system::default_queue());
  const uint8_t pattern = 0;
  compute::event event = queue.enqueue_mem_fill(
      source->get(), &pattern, sizeof(pattern), count * sizeof(uint8_t));
  queue.finish();

  EXPECT_EQ(CL_COMMAND_MEMFILL_INTEL, event.get_command_type());

  source->free();
}

} // namespace
