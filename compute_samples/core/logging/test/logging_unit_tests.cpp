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

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <regex>

namespace cs = compute_samples;

class LoggingTest : public ::testing::Test {
protected:
  void SetUp() override {
    cs::LoggingSettings settings;
    settings.level = cs::logging_level::trace;
    settings.format = cs::logging_format::simple;
    cs::init_logging(settings);
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

TEST(LoggingCommandLineParser, ChooseSimpleFormatFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-format=simple"};
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_format::simple, settings.format);
}

TEST(LoggingCommandLineParser, ChoosePreciseFormatFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-format=precise"};
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_format::precise, settings.format);
}

TEST(LoggingCommandLineParser, PreciseFormatIsDefault) {
  std::vector<std::string> cmd;
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_format::precise, settings.format);
}

TEST(LoggingCommandLineParser, ChooseUnknownFormatFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-format=unknown"};
  EXPECT_THROW(cs::parse_command_line(cmd), po::validation_error);
}

TEST(LoggingCommandLineParser, ConsumeOnlyKnownOptionsFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-format=precise",
                                  "positional_option", "--option"};
  cs::parse_command_line(cmd);
  EXPECT_EQ(2, cmd.size());
}

TEST(LoggingCommandLineParser, ChooseTraceLevelFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-level=trace"};
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_level::trace, settings.level);
}

TEST(LoggingCommandLineParser, ChooseDebugLevelFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-level=debug"};
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_level::debug, settings.level);
}

TEST(LoggingCommandLineParser, ChooseInfoLevelFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-level=info"};
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_level::info, settings.level);
}

TEST(LoggingCommandLineParser, ChooseWarningLevelFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-level=warning"};
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_level::warning, settings.level);
}

TEST(LoggingCommandLineParser, ChooseErrorLevelFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-level=error"};
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_level::error, settings.level);
}

TEST(LoggingCommandLineParser, ChooseFatalLevelFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-level=fatal"};
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_level::fatal, settings.level);
}

TEST(LoggingCommandLineParser, InfoLevelIsDefault) {
  std::vector<std::string> cmd;
  const cs::LoggingSettings settings = cs::parse_command_line(cmd);
  EXPECT_EQ(cs::logging_level::info, settings.level);
}

TEST(LoggingCommandLineParser, ChooseUnknownLevelFromCommandLine) {
  std::vector<std::string> cmd = {"--logging-level=unknown"};
  EXPECT_THROW(cs::parse_command_line(cmd), po::validation_error);
}

class LoggingInitTest : public ::testing::Test {
protected:
  void SetUp() override { logs = boost::make_shared<std::stringstream>(); }

  void TearDown() override { cs::stop_logging(); }

  boost::shared_ptr<std::stringstream> logs;
};

TEST_F(LoggingInitTest, SimpleFormatFromSettings) {
  cs::LoggingSettings settings;
  settings.format = cs::logging_format::simple;
  cs::init_logging(settings);
  cs::add_stream(logs);
  LOG_INFO << "Message";
  EXPECT_EQ("[info] Message\n", logs->str());
}

TEST_F(LoggingInitTest, PreciseFormatFromSettings) {
  cs::LoggingSettings settings;
  settings.format = cs::logging_format::precise;
  cs::init_logging(settings);
  cs::add_stream(logs);

  LOG_INFO << "Message";

  const std::string timestamp = "\\[.+\\]";
  const std::string severity = "\\[info\\]";
  const std::string message = "Message\\n";
  const std::regex r(timestamp + " " + severity + " " + message);
  EXPECT_TRUE(std::regex_match(logs->str(), r));
}

TEST_F(LoggingInitTest, WarningLevelFromSettings) {
  cs::LoggingSettings settings;
  settings.level = cs::logging_level::warning;
  settings.format = cs::logging_format::simple;
  cs::init_logging(settings);
  cs::add_stream(logs);

  LOG_INFO << "Message";
  EXPECT_EQ("", logs->str());
  LOG_WARNING << "Message";
  EXPECT_EQ("[warning] Message\n", logs->str());
}
