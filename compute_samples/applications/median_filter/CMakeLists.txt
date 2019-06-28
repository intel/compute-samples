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
