/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"

#include "logging/logging.hpp"
#include "test_harness/test_harness.hpp"

namespace cs = compute_samples;
namespace compute = boost::compute;

namespace {

const std::vector<compute::usm_type> atomic_memory_types = {
    compute::usm_type::host, compute::usm_type::device,
    compute::usm_type::shared, compute::usm_type::unknown};

class AtomicCopyTest : public ::testing::TestWithParam<
                           std::tuple<compute::usm_type, compute::usm_type>> {
protected:
  void SetUp() override {
    const compute::usm_type source_type = std::get<0>(GetParam());
    source_ = get_memory_wrapper<int>(source_type);

    const compute::usm_type destination_type = std::get<1>(GetParam());
    destination_ = get_memory_wrapper<int>(destination_type);

    if (!is_memory_access_supported(source_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(source_type));
    }
    if (!is_memory_access_supported(destination_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(destination_type));
    }
    if (!is_atomic_access_supported(source_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Atomic access is not supported for: " +
                                  cs::to_string(source_type));
    }
    if (!is_atomic_access_supported(destination_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Atomic access is not supported for: " +
                                  cs::to_string(destination_type));
    }

    source_->allocate(allocation_count_);
    destination_->allocate(allocation_count_);

    std::vector<int> source_data(allocation_count_);
    std::iota(source_data.begin(), source_data.end(), 0);
    source_->write(source_data);

    const std::vector<int> destination_data(allocation_count_, 0);
    destination_->write(destination_data);

    compute::context context = compute::system::default_context();
    compute::program program = cs::build_program(
        context, "test_cl_unified_shared_memory.cl", "-cl-std=CL2.0");
    kernel_ =
        compute::kernel_intel(program.create_kernel("atomic_copy_kernel"));

    kernel_.set_arg_mem_ptr(0, destination_->get());
    kernel_.set_arg_mem_ptr(1, source_->get());

    queue_ = compute::system::default_queue();
  }

  void TearDown() override {
    destination_->free();
    source_->free();
  }

  const int allocation_count_ = 256;
  compute::command_queue queue_;
  compute::kernel_intel kernel_;
  std::unique_ptr<MemoryWrapper<int>> source_;
  std::unique_ptr<MemoryWrapper<int>> destination_;
};

HWTEST_P(AtomicCopyTest, AtomicLoadsAndStoresAreProperlyHandled) {
  const std::vector<int> reference = source_->read();

  queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0);
  queue_.finish();

  const std::vector<int> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

std::string atomic_copy_test_name_suffix(
    testing::TestParamInfo<std::tuple<compute::usm_type, compute::usm_type>>
        param_info) {
  std::stringstream ss;
  ss << "source_" << std::get<0>(param_info.param) << "_";
  ss << "destination_" << std::get<1>(param_info.param);
  return ss.str();
}

INSTANTIATE_TEST_SUITE_P(
    MemoryTypes, AtomicCopyTest,
    ::testing::Combine(::testing::ValuesIn(atomic_memory_types),
                       ::testing::ValuesIn(atomic_memory_types)),
    atomic_copy_test_name_suffix);
} // namespace
