/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"

#include "boost/compute/intel/command_queue.hpp"

#include "utils/utils.hpp"
#include "test_harness/test_harness.hpp"

namespace cs = compute_samples;
namespace compute = boost::compute;

namespace {

const std::vector<compute::usm_type> indirect_memory_types = {
    compute::usm_type::host, compute::usm_type::device,
    compute::usm_type::shared, compute::usm_type::unknown};

class IndirectCopyTest : public ::testing::TestWithParam<
                             std::tuple<compute::usm_type, compute::usm_type,
                                        compute::usm_type, compute::usm_type>> {
protected:
  void SetUp() override {
    source_type_ = std::get<0>(GetParam());
    source_ = get_memory_wrapper<uint8_t>(source_type_);

    destination_type_ = std::get<1>(GetParam());
    destination_ = get_memory_wrapper<uint8_t>(destination_type_);

    const compute::usm_type source_pointers_type = std::get<2>(GetParam());
    source_pointers_ = get_memory_wrapper<uint8_t *>(source_pointers_type);

    const compute::usm_type destination_pointers_type = std::get<3>(GetParam());
    destination_pointers_ =
        get_memory_wrapper<uint8_t *>(destination_pointers_type);

    if (!is_memory_access_supported(source_type_)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(source_type_));
    }
    if (!is_memory_access_supported(destination_type_)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(destination_type_));
    }
    if (!is_memory_access_supported(source_pointers_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(source_pointers_type));
    }
    if (!is_memory_access_supported(destination_pointers_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(destination_pointers_type));
    }

    source_->allocate(allocation_count_);
    destination_->allocate(allocation_count_);

    source_pointers_->allocate(allocation_count_);
    destination_pointers_->allocate(allocation_count_);

    std::vector<uint8_t> source_data(allocation_count_);
    std::iota(source_data.begin(), source_data.end(), 0u);
    source_->write(source_data);

    const std::vector<uint8_t> destination_data(allocation_count_, 0u);
    destination_->write(destination_data);

    std::vector<uint8_t *> source_pointers_data(allocation_count_);
    std::vector<uint8_t *> destination_pointers_data(allocation_count_);
    for (int i = 0; i < allocation_count_; ++i) {
      source_pointers_data[i] = source_->get() + i;
      destination_pointers_data[i] = destination_->get() + i;
    }
    source_pointers_->write(source_pointers_data);
    destination_pointers_->write(destination_pointers_data);

    compute::context context = compute::system::default_context();
    compute::program program = cs::build_program(
        context, "test_cl_unified_shared_memory.cl", "-cl-std=CL2.0");
    kernel_ =
        compute::kernel_intel(program.create_kernel("indirect_copy_kernel"));

    kernel_.set_arg_mem_ptr(0, destination_pointers_->get());
    kernel_.set_arg_mem_ptr(1, source_pointers_->get());

    queue_ = compute::system::default_queue();
  }

  void TearDown() override {
    destination_->free();
    source_->free();
  }

  const int allocation_count_ = 256;
  compute::command_queue queue_;
  compute::kernel_intel kernel_;
  std::unique_ptr<MemoryWrapper<uint8_t>> source_;
  std::unique_ptr<MemoryWrapper<uint8_t>> destination_;
  std::unique_ptr<MemoryWrapper<uint8_t *>> source_pointers_;
  std::unique_ptr<MemoryWrapper<uint8_t *>> destination_pointers_;
  compute::usm_type source_type_;
  compute::usm_type destination_type_;
};

HWTEST_P(IndirectCopyTest,
         GivenIndirectPointersExplicitlyThenValuesAreProperlyCopied) {
  const std::vector<uint8_t> reference = source_->read();

  const std::vector<void *> indirect_pointers = {source_->get(),
                                                 destination_->get()};
  kernel_.set_exec_info(CL_KERNEL_EXEC_INFO_USM_PTRS_INTEL,
                        cs::size_in_bytes(indirect_pointers),
                        indirect_pointers.data());
  queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0);
  queue_.finish();

  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

HWTEST_P(IndirectCopyTest,
         GivenIndirectPointersImplicitlyThenValuesAreProperlyCopied) {
  const std::vector<uint8_t> reference = source_->read();

  const cl_bool value = CL_TRUE;
  if (source_type_ == compute::usm_type::host ||
      destination_type_ == compute::usm_type::host) {
    kernel_.set_exec_info(CL_KERNEL_EXEC_INFO_INDIRECT_HOST_ACCESS_INTEL,
                          sizeof(value), &value);
  }
  if (source_type_ == compute::usm_type::device ||
      destination_type_ == compute::usm_type::device) {
    kernel_.set_exec_info(CL_KERNEL_EXEC_INFO_INDIRECT_DEVICE_ACCESS_INTEL,
                          sizeof(value), &value);
  }
  if (source_type_ == compute::usm_type::shared ||
      destination_type_ == compute::usm_type::shared) {
    kernel_.set_exec_info(CL_KERNEL_EXEC_INFO_INDIRECT_SHARED_ACCESS_INTEL,
                          sizeof(value), &value);
  }

  queue_.enqueue_1d_range_kernel(kernel_, 0, allocation_count_, 0);
  queue_.finish();

  const std::vector<uint8_t> actual = destination_->read(allocation_count_);
  EXPECT_EQ(reference, actual);
}

std::string indirect_access_test_name_suffix(
    testing::TestParamInfo<std::tuple<compute::usm_type, compute::usm_type,
                                      compute::usm_type, compute::usm_type>>
        param_info) {
  std::stringstream ss;
  ss << "source_" << std::get<0>(param_info.param) << "_";
  ss << "destination_" << std::get<1>(param_info.param) << "_";
  ss << "source_pointers_" << std::get<2>(param_info.param) << "_";
  ss << "destination_pointers_" << std::get<3>(param_info.param);
  return ss.str();
}
INSTANTIATE_TEST_SUITE_P(
    MemoryTypes, IndirectCopyTest,
    ::testing::Combine(::testing::ValuesIn(indirect_memory_types),
                       ::testing::ValuesIn(indirect_memory_types),
                       ::testing::ValuesIn(indirect_memory_types),
                       ::testing::ValuesIn(indirect_memory_types)),
    indirect_access_test_name_suffix);

} // namespace
