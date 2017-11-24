cmake_minimum_required(VERSION 3.8)
project(vme_intra)

set(SOURCES
    include/vme_intra/vme_intra.hpp
    src/vme_intra.cpp
    vme_intra_0_tier.cl
    vme_intra.cl
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    compute_samples::timer
    Boost::log
    Boost::program_options
    compute_samples::application
    compute_samples::yuv_utils
    compute_samples::align_utils
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

file(COPY "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv" DESTINATION ${CMAKE_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/../vme_hme/downsample_3_tier.cl" ${CMAKE_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/../vme_hme/vme_hme_n_tier.cl" ${CMAKE_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/vme_intra_0_tier.cl" ${CMAKE_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/vme_intra.cl" ${CMAKE_BINARY_DIR})

install(FILES "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv" DESTINATION ${BIN_DIRECTORY})
install(FILES "${PROJECT_SOURCE_DIR}/vme_intra_0_tier.cl" DESTINATION ${BIN_DIRECTORY})
install(FILES "${PROJECT_SOURCE_DIR}/vme_intra.cl" DESTINATION ${BIN_DIRECTORY})

source_group("Kernel Files" FILES downsample_3_tier.cl vme_streaminout.cl vme_hme_n_tier.cl vme_intra_0_tier.cl vme_intra.cl)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER applications/${PROJECT_NAME})

set(TEST_SOURCES
    test/vme_intra_system_tests.cpp
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
add_executable(compute_samples::${PROJECT_NAME}_tests ALIAS ${PROJECT_NAME}_tests)

target_link_libraries(${PROJECT_NAME}_tests
    PUBLIC
    ${PROJECT_NAME}
    compute_samples::framework
    GTest::GTest
    GTest::Main
)

set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER applications/${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/../vme_hme/downsample_3_tier.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/../vme_hme/vme_streaminout.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/../vme_hme/vme_hme_n_tier.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/vme_intra_0_tier.cl" ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/vme_intra.cl" ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/yuv/intra_foreman_176x144.yuv" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME}_tests DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv" DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/yuv/intra_foreman_176x144.yuv" DESTINATION ${BIN_DIRECTORY})

add_test(${PROJECT_NAME}_tests, ${PROJECT_NAME}_tests)
