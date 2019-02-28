#
# Copyright(c) 2018 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required(VERSION 3.8)
project(image)

add_core_library(${PROJECT_NAME}
    SOURCE
    "include/image/image.hpp"
    "src/image.cpp"
    "src/bmp.hpp"
    "src/bmp.cpp"
)
target_include_directories(${PROJECT_NAME}
	PRIVATE
    "src"
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    compute_samples::logging
    PRIVATE
    Boost::boost
    PNG::PNG
)

add_core_library_test(${PROJECT_NAME}
    SOURCE
    "test/main.cpp"
    "test/image_integration_tests.cpp"
)
install_resources(${PROJECT_NAME}_tests
    FILES
    "${MEDIA_DIRECTORY}/png/rgb_brg_3x2.png"
    "${MEDIA_DIRECTORY}/bmp/kwkw_wwkk_4x2_mono.bmp"
    "${MEDIA_DIRECTORY}/bmp/rgb_brg_3x2_argb.bmp"
)
