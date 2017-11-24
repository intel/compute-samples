cmake_minimum_required(VERSION 3.8)
project(median_filter)

set(SOURCES
    include/median_filter/median_filter.hpp
    src/median_filter.cpp
    median_filter.cl
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
    compute_samples::application
    compute_samples::image
    Boost::program_options
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

file(COPY "${MEDIA_DIRECTORY}/png/balloons_news.png" DESTINATION ${CMAKE_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/median_filter.cl" ${CMAKE_BINARY_DIR})

install(FILES "${PROJECT_SOURCE_DIR}/median_filter.cl" DESTINATION ${BIN_DIRECTORY})

source_group("Kernel Files" FILES median_filter.cl)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER applications/${PROJECT_NAME})

set(TEST_SOURCES
    test/median_filter_system_tests.cpp
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

file(COPY "${MEDIA_DIRECTORY}/png/test_input.png" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/png/test_reference.png" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/median_filter.cl" ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME}_tests DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/png/test_input.png" DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/png/test_reference.png" DESTINATION ${BIN_DIRECTORY})

add_test(${PROJECT_NAME}_tests, ${PROJECT_NAME}_tests)
