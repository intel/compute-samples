#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(image
    SOURCE
    "include/image/image.hpp"
    "src/image.cpp"
    "src/bmp.hpp"
    "src/bmp.cpp"
)
target_include_directories(image
    PRIVATE
    "src"
)
target_link_libraries(image
    PUBLIC
    compute_samples::logging
    PRIVATE
    Boost::boost
    PNG::PNG
)

add_core_library_test(image
    SOURCE
    "test/main.cpp"
    "test/image_unit_tests.cpp"
    "test/image_integration_tests.cpp"
)
install_resources(image_tests
    FILES
    "${MEDIA_DIRECTORY}/png/rgb_brg_3x2.png"
    "${MEDIA_DIRECTORY}/bmp/kwkw_wwkk_4x2_mono.bmp"
    "${MEDIA_DIRECTORY}/bmp/rgb_brg_3x2_argb.bmp"
)
