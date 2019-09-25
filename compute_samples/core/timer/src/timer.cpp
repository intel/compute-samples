/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "timer/timer.hpp"
#include "logging/logging.hpp"

namespace timer = boost::timer;

namespace compute_samples {
Timer::Timer() { timer_.start(); }

void Timer::print(const std::string &event_name) {
  timer_.stop();
  LOG_INFO << event_name << ": "
           << timer_.format(timer::default_places, timer_format_);
  timer_.start();
}
} // namespace compute_samples
