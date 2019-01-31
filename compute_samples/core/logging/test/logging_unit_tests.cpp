/*
 * Copyright(c) 2018 Intel Corporation
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

#include "logging/logging.hpp"
#include "gtest/gtest.h"

#include <boost/smart_ptr/make_shared_object.hpp>

namespace cs = compute_samples;

class LoggingTest : public ::testing::Test {
protected:
  void SetUp() override {
    cs::init_logging();
    cs::set_simple_format();
    logs = boost::make_shared<std::stringstream>();
    cs::add_stream(logs);
  }

  void TearDown() override { cs::stop_logging(); }

  boost::shared_ptr<std::stringstream> logs;
};

TEST_F(LoggingTest, PrintTrace) {
  LOG_TRACE << "Message";
  EXPECT_EQ("[trace] Message\n", logs->str());
}

TEST_F(LoggingTest, PrintDebug) {
  LOG_DEBUG << "Message";
  EXPECT_EQ("[debug] Message\n", logs->str());
}

TEST_F(LoggingTest, PrintInfo) {
  LOG_INFO << "Message";
  EXPECT_EQ("[info] Message\n", logs->str());
}

TEST_F(LoggingTest, PrintWarning) {
  LOG_WARNING << "Message";
  EXPECT_EQ("[warning] Message\n", logs->str());
}

TEST_F(LoggingTest, PrintError) {
  LOG_ERROR << "Message";
  EXPECT_EQ("[error] Message\n", logs->str());
}

TEST_F(LoggingTest, PrintFatal) {
  LOG_FATAL << "Message";
  EXPECT_EQ("[fatal] Message\n", logs->str());
}
