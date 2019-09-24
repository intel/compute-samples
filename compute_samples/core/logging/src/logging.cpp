/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
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

#include <boost/program_options.hpp>
namespace po = boost::program_options;

namespace compute_samples {

typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
static boost::shared_ptr<text_sink> sink;

void set_format(const logging_format format) {
  logging::formatter formatter;
  if (format == logging_format::simple) {
    formatter = expr::stream << '[' << logging::trivial::severity << ']' << ' '
                             << expr::smessage;
  } else if (format == logging_format::precise) {
    formatter = expr::stream
                << '['
                << expr::format_date_time<boost::posix_time::ptime>(
                       "TimeStamp", "%Y-%m-%d %H:%M:%S")
                << ']' << ' ' << '[' << logging::trivial::severity << ']' << ' '
                << expr::smessage;
  } else {
    throw std::runtime_error("Unknown logging_format");
  }
  sink->set_formatter(formatter);
}

void set_min_level(const logging_level level) {
  logging::core::get()->set_filter(logging::trivial::severity >= level);
}

void init_logging() {
  sink = logging::add_console_log();
  logging::add_common_attributes();
}

void init_logging(const LoggingSettings settings) {
  init_logging();

  set_format(settings.format);
  set_min_level(settings.level);
}

void init_logging(std::vector<std::string> &command_line) {
  init_logging(parse_command_line(command_line));
}

void stop_logging() {
  logging::core::get()->remove_sink(sink);
  sink.reset();
}

void add_stream(const boost::shared_ptr<std::ostream> &stream) {
  sink->locked_backend()->add_stream(stream);
}

std::ostream &operator<<(std::ostream &os, const logging_format &f) {
  if (f == logging_format::simple) {
    os << "simple";
  } else if (f == logging_format::precise) {
    os << "precise";
  } else {
    throw std::runtime_error("Unknown logging_format");
  }
  return os;
}

std::istream &operator>>(std::istream &is, logging_format &f) {
  std::string s;
  is >> s;
  if (s == "simple") {
    f = logging_format::simple;
  } else if (s == "precise") {
    f = logging_format::precise;
  } else {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}

LoggingSettings parse_command_line(std::vector<std::string> &command_line) {
  LoggingSettings settings;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("logging-format",
          po::value(&settings.format)->default_value(logging_format::precise),
          "format of logged messages");
  options("logging-level",
          po::value(&settings.level)->default_value(logging_level::info),
          "minimal logging level to print");

  po::parsed_options parsed = po::command_line_parser(command_line)
                                  .options(desc)
                                  .allow_unregistered()
                                  .run();
  po::variables_map vm;
  po::store(parsed, vm);
  po::notify(vm);

  command_line =
      po::collect_unrecognized(parsed.options, po::include_positional);
  return settings;
}

} // namespace compute_samples
