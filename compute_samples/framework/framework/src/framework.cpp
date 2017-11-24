/*
 * Copyright(c) 2017 Intel Corporation
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

#include "framework/framework.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <type_traits>
#include <utility>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
namespace src = boost::log::sources;

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"
#include "median_filter/median_filter.hpp"
#include "subgroups_imagecopy/subgroups_imagecopy.hpp"
#include "subgroups_visualization/subgroups_visualization.hpp"
#include "vme_hme/vme_hme.hpp"
#include "vme_interlaced/vme_interlaced.hpp"
#include "vme_interop/vme_interop.hpp"
#include "vme_intra/vme_intra.hpp"
#include "vme_search/vme_search.hpp"
#include "vme_wpp/vme_wpp.hpp"

namespace compute_samples {

class ApplicationFactory {
public:
  std::unique_ptr<Application> create(const std::string id) const {
    return applications.at(id)();
  }
  std::vector<std::string> keys() const {
    std::vector<std::string> k;
    for (const auto &a : applications) {
      k.push_back(a.first);
    }
    return k;
  }
  std::string keys_string() const {
    const std::vector<std::string> k = keys();
    std::stringstream s;
    std::copy(k.begin(), k.end(), std::ostream_iterator<std::string>(s, "\n"));
    return s.str();
  }

private:
  const std::map<std::string, std::function<std::unique_ptr<Application>()>>
      applications{
          {"median_filter",
           []() {
             return std::unique_ptr<Application>{new MedianFilterApplication};
           }},
          {"subgroups_imagecopy",
           []() {
             return std::unique_ptr<Application>{
                 new SubgroupsImageCopyApplication};
           }},
          {"subgroups_visualization",
           []() {
             return std::unique_ptr<Application>{
                 new SubgroupsVisualizationApplication};
           }},
          {"vme_search",
           []() {
             return std::unique_ptr<Application>{new VmeSearchApplication};
           }},
          {"vme_hme",
           []() {
             return std::unique_ptr<Application>{new VmeHmeApplication};
           }},
          {"vme_wpp",
           []() {
             return std::unique_ptr<Application>{new VmeWppApplication};
           }},
          {"vme_intra",
           []() {
             return std::unique_ptr<Application>{new VmeIntraApplication};
           }},
          {"vme_interlaced",
           []() {
             return std::unique_ptr<Application>{new VmeInterlacedApplication};
           }},
          {"vme_interop", []() {
             return std::unique_ptr<Application>{new VmeInteropApplication};
           }}};
};

struct ApplicationOption {
  ApplicationOption(const std::string &val) : value(val) {}
  std::string value;
};

void validate(boost::any &v, const std::vector<std::string> &values,
              ApplicationOption *, int) {
  po::validators::check_first_occurrence(v);
  const std::string &s = po::validators::get_single_string(values);
  const std::vector<std::string> application_ids = ApplicationFactory().keys();
  if (std::find(application_ids.begin(), application_ids.end(), s) !=
      application_ids.end()) {
    v = boost::any(ApplicationOption(s));
  } else {
    throw po::error(
        "Unknown applicaion ID. Use --list-applications for available IDs");
  }
}

int Framework::run(int argc, const char **argv) {
  src::logger logger;

  try {
    Arguments args;
    std::vector<std::string> rest;
    const ApplicationFactory application_factory;
    std::tie(args, rest) =
        parse_command_line(argc, argv, application_factory.keys_string());

    if (args.help || args.list_applications) {
      return 0;
    }

    std::unique_ptr<Application> application =
        application_factory.create(args.application_id);

#ifndef _WIN32
#define PUTENV putenv
#else
#define PUTENV _putenv
#endif
    char set_default_vendor[] = "BOOST_COMPUTE_DEFAULT_VENDOR=Intel";
    char set_default_device[] = "BOOST_COMPUTE_DEFAULT_DEVICE_TYPE=GPU";
    PUTENV(set_default_vendor);
    PUTENV(set_default_device);

    application->run(rest, logger);
  } catch (const std::exception &e) {
    BOOST_LOG(logger) << e.what();
    return 1;
  }

  return 0;
}

std::tuple<Framework::Arguments, std::vector<std::string>>
Framework::parse_command_line(int argc, const char **argv,
                              const std::string available_applications) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message - to show application help "
                    "add -- -h after application-id");
  options("application-id", po::value<ApplicationOption>()->required(),
          "ID of application to run");
  options("application-args", po::value<std::vector<std::string>>(),
          "Arguments for application");
  options("list-applications,l", "List all available application IDs");

  po::positional_options_description p;
  p.add("application-id", 1);
  p.add("application-args", -1);

  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argc, argv)
                                  .options(desc)
                                  .positional(p)
                                  .allow_unregistered()
                                  .run();
  po::store(parsed, vm);

  if (vm.count("help")) {
    std::cout << desc;
    args.help = true;
    return std::make_tuple(args, std::vector<std::string>());
  }

  if (vm.count("list-applications")) {
    std::cout << "Available applications:\n" << available_applications;
    args.list_applications = true;
    return std::make_tuple(args, std::vector<std::string>());
  }

  po::notify(vm);
  args.application_id = vm["application-id"].as<ApplicationOption>().value;

  std::vector<std::string> rest =
      po::collect_unrecognized(parsed.options, po::include_positional);
  if (!rest.empty()) {
    rest.erase(rest.begin());
  }

  return std::make_tuple(args, rest);
}
} // namespace compute_samples
