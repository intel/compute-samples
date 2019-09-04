#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(vme_wpp
    SOURCE
    "include/vme_wpp/vme_wpp.hpp"
    "src/vme_wpp.cpp"
)
target_link_libraries(vme_wpp_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::yuv_utils
    compute_samples::align_utils
    compute_samples::ocl_utils
)
add_kernels(vme_wpp_lib
    "scoreboard.cl"
    "vme_wpp_0_tier.cl"
    "vme_wpp.cl"
)

add_application(vme_wpp
    SOURCE
    "src/main.cpp"
)
install_kernels(vme_wpp
    "scoreboard.cl"
    "vme_wpp_0_tier.cl"
    "vme_wpp.cl"
    "../vme_hme/downsample_3_tier.cl"
    "../vme_hme/vme_streaminout.cl"
    "../vme_hme/vme_hme_n_tier.cl"
    "../vme_hme/vme_hme_0_tier.cl"
)
install_resources(vme_wpp FILES "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv")

add_application_test(vme_wpp
    SOURCE
    "test/main.cpp"
    "test/vme_wpp_integration_tests.cpp"
    "test/vme_wpp_system_tests.cpp"
)
install_kernels(vme_wpp_tests
    "scoreboard.cl"
    "vme_wpp_0_tier.cl"
    "vme_wpp.cl"
    "../vme_hme/downsample_3_tier.cl"
    "../vme_hme/vme_streaminout.cl"
    "../vme_hme/vme_hme_n_tier.cl"
    "../vme_hme/vme_hme_0_tier.cl"
)
install_resources(vme_wpp_tests
    FILES
    "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv"
    "${MEDIA_DIRECTORY}/yuv/wpp_foreman_176x144.yuv"
)
