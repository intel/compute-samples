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

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;

namespace compute_samples {

typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
static boost::shared_ptr<text_sink> sink;

void init_logging() {
  sink = logging::add_console_log();
  logging::add_common_attributes();
}

void stop_logging() {
  logging::core::get()->remove_sink(sink);
  sink.reset();
}

void add_stream(const boost::shared_ptr<std::ostream> &stream) {
  sink->locked_backend()->add_stream(stream);
}

void set_simple_format() {
  sink->set_formatter(expr::stream << '[' << logging::trivial::severity << ']'
                                   << ' ' << expr::smessage);
}

void set_precise_format() {
  sink->set_formatter(expr::stream
                      << '['
                      << expr::format_date_time<boost::posix_time::ptime>(
                             "TimeStamp", "%Y-%m-%d %H:%M:%S")
                      << ']' << ' ' << '[' << logging::trivial::severity << ']'
                      << ' ' << expr::smessage);
}

} // namespace compute_samples
