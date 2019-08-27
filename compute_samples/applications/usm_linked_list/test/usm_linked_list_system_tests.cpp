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
#include "test_harness/test_harness.hpp"

TEST(UsmLinkedListSystemTests,
     ApplicationReturnsSkipStatusGivenHelpMessageIsRequested) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"--help"};
  EXPECT_EQ(compute_samples::Application::Status::SKIP,
            application.run(command_line));
}

HWTEST(UsmLinkedListSystemTests, GivenHostUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"host"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmLinkedListSystemTests, GivenDeviceUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"device"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmLinkedListSystemTests, GivenSharedUsmThenApplicationReturnsOKStatus) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"shared"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}

HWTEST(UsmLinkedListSystemTests,
       GivenCustomListSizeThenApplicationReturnsOKStatus) {
  compute_samples::UsmLinkedListApplication application;
  std::vector<std::string> command_line = {"host", "--size", "1024"};
  EXPECT_EQ(compute_samples::Application::Status::OK,
            application.run(command_line));
}
