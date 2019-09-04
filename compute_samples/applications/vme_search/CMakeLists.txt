#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(vme_search
    SOURCE
    "include/vme_search/vme_search.hpp"
    "src/vme_search.cpp"
)
target_link_libraries(vme_search_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::yuv_utils
    compute_samples::align_utils
    compute_samples::ocl_utils
)
add_kernels(vme_search_lib
    "vme_basic_search.cl"
    "vme_cost_heuristics_search.cl"
    "vme_larger_search.cl"
)

add_application(vme_search
    SOURCE
    "src/main.cpp"
)
install_kernels(vme_search
    "vme_basic_search.cl"
    "vme_cost_heuristics_search.cl"
    "vme_larger_search.cl"
)
install_resources(vme_search FILES "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv")

add_application_test(vme_search
    SOURCE
    "test/main.cpp"
    "test/vme_search_integration_tests.cpp"
    "test/vme_search_system_tests.cpp"
)
install_kernels(vme_search_tests
    "vme_basic_search.cl"
    "vme_cost_heuristics_search.cl"
    "vme_larger_search.cl"
)
install_resources(vme_search_tests
    FILES
    "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv"
    "${MEDIA_DIRECTORY}/yuv/basic_search_foreman_176x144.yuv"
    "${MEDIA_DIRECTORY}/yuv/cost_heuristics_search_foreman_176x144.yuv"
    "${MEDIA_DIRECTORY}/yuv/larger_search_foreman_176x144.yuv"
)
