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

#ifndef COMPUTE_SAMPLES_USM_LINKED_LIST_HPP
#define COMPUTE_SAMPLES_USM_LINKED_LIST_HPP

#include <vector>

#include <boost/compute/core.hpp>
#include <boost/compute/intel/command_queue.hpp>
#include <boost/compute/intel/kernel.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"
#include "ocl_utils/unified_shared_memory.hpp"

namespace compute_samples {

class UsmLinkedListApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool help = false;
    compute::usm_type type = compute::usm_type::host;
    int size = 0;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
};

struct Node {
  Node *next = nullptr;
  cl_uint value = 0u;
};

template <typename T>
T *allocate_memory(const compute::usm_type type, const int size = 1) {
  compute::context context(compute::system::default_context());
  compute::device device(compute::system::default_device());

  T *p = nullptr;
  if (type == compute::usm_type::host) {
    p = compute::host_mem_alloc<T>(context, nullptr, size, 0);
  } else if (type == compute::usm_type::device) {
    p = compute::device_mem_alloc<T>(context, device, nullptr, size, 0);
  } else if (type == compute::usm_type::shared) {
    p = compute::shared_mem_alloc<T>(context, device, nullptr, size, 0);
  } else {
    throw std::runtime_error("Unknown USM type: " +
                             std::to_string(static_cast<int>(type)));
  }

  return p;
}

template <typename T> T read_memory(const T *p, const compute::usm_type type) {
  if (type == compute::usm_type::device) {
    T tmp;
    compute::command_queue_intel queue(compute::system::default_queue());
    queue.enqueue_memcpy(&tmp, p, sizeof(T));
    return tmp;
  }
  return *p;
}

compute::kernel_intel prepare_kernel(const compute::usm_type type);

Node *allocate_linked_list(const int size, const compute::usm_type type);
void walk_linked_list(Node *head, compute::kernel_intel &kernel);
void free_linked_list(Node *head);

} // namespace compute_samples

#endif
