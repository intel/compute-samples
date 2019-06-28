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
