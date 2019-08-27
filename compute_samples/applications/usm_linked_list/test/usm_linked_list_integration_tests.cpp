/*
 * Copyright(c) 2019 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "gtest/gtest.h"
#include "usm_linked_list/usm_linked_list.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "test_harness/test_harness.hpp"
namespace cs = compute_samples;

class UsmLinkedListIntegrationTests
    : public ::testing::TestWithParam<compute::usm_type> {};

HWTEST(UsmLinkedListIntegrationTests, ProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(context, "usm_linked_list.cl"));
}

HWTEST_P(UsmLinkedListIntegrationTests, PrepareKernel) {
  EXPECT_NO_THROW(cs::prepare_kernel(GetParam()));
}

HWTEST_P(UsmLinkedListIntegrationTests, AllocateMemory) {
  cs::Node *p = cs::allocate_memory<cs::Node>(GetParam());
  EXPECT_NE(nullptr, p);

  compute::context context(compute::system::default_context());
  compute::mem_free(context, p);
}

HWTEST_P(UsmLinkedListIntegrationTests, AllocateLinkedList) {
  const int size = 10;
  cs::Node *head = cs::allocate_linked_list(size, GetParam());

  int actual = 0;
  cs::Node *current = head;
  while (current != nullptr) {
    actual++;
    current = current->next;
  }
  EXPECT_EQ(size, actual);

  cs::free_linked_list(head);
}

HWTEST_P(UsmLinkedListIntegrationTests, FreeLinkedList) {
  const int size = 10;
  cs::Node *head = cs::allocate_linked_list(size, GetParam());
  EXPECT_NO_THROW(cs::free_linked_list(head));
}

HWTEST_P(UsmLinkedListIntegrationTests, WalkLinkedList) {
  const int size = 10;
  cs::Node *head = cs::allocate_linked_list(size, GetParam());
  compute::kernel_intel kernel = cs::prepare_kernel(GetParam());

  cs::walk_linked_list(head, kernel);

  cs::Node *current = head;
  int count = 0;
  while (current != nullptr) {
    EXPECT_EQ(count * 4 + 1, cs::read_memory(current, GetParam()).value);
    current = current->next;
    count++;
  }

  cs::free_linked_list(head);
}

INSTANTIATE_TEST_CASE_P(UsmTypes, UsmLinkedListIntegrationTests,
                        ::testing::Values(compute::usm_type::host,
                                          compute::usm_type::device,
                                          compute::usm_type::shared));
