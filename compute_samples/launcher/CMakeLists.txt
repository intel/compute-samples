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
project(launcher)

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    compute_samples::framework
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

file(COPY "${CMAKE_BINARY_DIR}/balloons_news.png" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/median_filter.cl" ${CMAKE_CURRENT_BINARY_DIR})

file(COPY "${CMAKE_BINARY_DIR}/input.bmp" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${CMAKE_BINARY_DIR}/subgroups_imagecopy_solution.cl" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/subgroups_imagecopy_kernel.cl" ${CMAKE_CURRENT_BINARY_DIR})

file(COPY "${CMAKE_BINARY_DIR}/subgroups_visualization_solution.cl" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/subgroups_visualization_kernel.cl" ${CMAKE_CURRENT_BINARY_DIR})

file(COPY "${CMAKE_BINARY_DIR}/goal_1280x720.yuv" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_basic_search.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_cost_heuristics_search.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_larger_search.cl" ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/downsample_3_tier.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_streaminout.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_hme_n_tier.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_hme_0_tier.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_hme.cl" ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_intra_0_tier.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_intra.cl" ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/scoreboard.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_wpp_0_tier.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_wpp.cl" ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_interlaced.cl" ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_BINARY_DIR}/vme_interop.cl" ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME} DESTINATION ".")
