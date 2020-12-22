#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(vme_interop
    SOURCE
    "include/vme_interop/vme_interop.hpp"
    "src/vme_interop.cpp"
)
target_link_libraries(vme_interop_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::yuv_utils
    compute_samples::align_utils
    compute_samples::boost_intel
    compute_samples::ocl_utils
)
add_kernels(vme_interop_lib "vme_interop.cl")
if(UNIX)
    target_sources(vme_interop_lib PRIVATE "include/vme_interop/vme_interop_linux.hpp" "src/vme_interop_linux.cpp")
    target_link_libraries(vme_interop_lib PUBLIC compute_samples::va_utils)
endif()
if(WIN32)
    target_sources(vme_interop_lib PRIVATE "src/vme_interop_windows.cpp")
endif()

add_application(vme_interop
    SOURCE
    "src/main.cpp"
)
install_kernels(vme_interop "vme_interop.cl")
install_resources(vme_interop FILES "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv")

add_application_test(vme_interop
    SOURCE
    "test/main.cpp"
    "test/vme_interop_integration_tests.cpp"
    "test/vme_interop_system_tests_common.hpp"
)
install_kernels(vme_interop_tests "vme_interop.cl")
install_resources(vme_interop_tests
    FILES
    "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv"
    "${MEDIA_DIRECTORY}/yuv/interop_foreman_176x144.yuv"
)
if(UNIX)
    target_sources(vme_interop_tests PRIVATE "test/vme_interop_system_tests_linux.cpp")
endif()
if(WIN32)
    target_sources(vme_interop_tests PRIVATE "test/vme_interop_system_tests_windows.cpp")
endif()
