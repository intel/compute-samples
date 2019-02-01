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

#ifndef COMPUTE_SAMPLES_LOGGING_HPP
#define COMPUTE_SAMPLES_LOGGING_HPP

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace compute_samples {

#define LOG_TRACE BOOST_LOG_TRIVIAL(trace)
#define LOG_DEBUG BOOST_LOG_TRIVIAL(debug)
#define LOG_INFO BOOST_LOG_TRIVIAL(info)
#define LOG_WARNING BOOST_LOG_TRIVIAL(warning)
#define LOG_ERROR BOOST_LOG_TRIVIAL(error)
#define LOG_FATAL BOOST_LOG_TRIVIAL(fatal)

#define LOG_ENTER_FUNCTION LOG_TRACE << "Enter function: " << __func__;
#define LOG_EXIT_FUNCTION LOG_TRACE << "Exit function: " << __func__;

enum class logging_format { simple, precise };
std::ostream &operator<<(std::ostream &os, const logging_format &f);
std::istream &operator>>(std::istream &is, logging_format &f);

using logging_level = boost::log::trivial::severity_level;

struct LoggingSettings {
  logging_format format;
  logging_level level;
};

void init_logging();
void init_logging(const LoggingSettings settings);
void init_logging(std::vector<std::string> &command_line);
void stop_logging();
void add_stream(const boost::shared_ptr<std::ostream> &stream);
LoggingSettings parse_command_line(std::vector<std::string> &command_line);

template <typename T> std::string to_string(const std::vector<T> &x) {
  std::stringstream ss;
  ss << '[';
  if (!x.empty()) {
    std::copy(x.begin(), x.end() - 1, std::ostream_iterator<T>(ss, ", "));
    ss << x.back();
  }
  ss << ']';
  return ss.str();
}

} // namespace compute_samples

#endif
