/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_TIMER_HPP
#define COMPUTE_SAMPLES_TIMER_HPP

#include <boost/timer/timer.hpp>

namespace compute_samples {
class Timer {
public:
  Timer();
  void print(const std::string &event_name);

private:
  boost::timer::cpu_timer timer_;
  const std::string timer_format_ = "%ws";
};
} // namespace compute_samples

#endif
