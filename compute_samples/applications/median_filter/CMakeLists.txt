#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(median_filter
    SOURCE
    "include/median_filter/median_filter.hpp"
    "src/median_filter.cpp"
)
target_link_libraries(median_filter_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::image
    compute_samples::ocl_utils
)
add_kernels(median_filter_lib "median_filter.cl")

add_application(median_filter
    SOURCE
    "src/main.cpp"
)
install_kernels(median_filter "median_filter.cl")
install_resources(median_filter FILES "${MEDIA_DIRECTORY}/png/balloons_news.png")

add_application_test(median_filter
    SOURCE
    "test/main.cpp"
    "test/median_filter_integration_tests.cpp"
    "test/median_filter_system_tests.cpp"
)
install_kernels(median_filter_tests "median_filter.cl")
install_resources(median_filter_tests
    FILES
    "${MEDIA_DIRECTORY}/png/test_input.png"
    "${MEDIA_DIRECTORY}/png/test_reference.png"
)
