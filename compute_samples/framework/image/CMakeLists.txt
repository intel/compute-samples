cmake_minimum_required(VERSION 3.8)
project(image)

set(SOURCES
    include/image/image.hpp
    src/image.cpp
    src/bmp.hpp
    src/bmp.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
	PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Boost::boost
    PNG::PNG
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER framework/${PROJECT_NAME})

set(TEST_SOURCES
    test/image_integration_tests.cpp
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
add_executable(compute_samples::${PROJECT_NAME}_tests ALIAS ${PROJECT_NAME}_tests)

target_link_libraries(${PROJECT_NAME}_tests
    PUBLIC
    ${PROJECT_NAME}
    GTest::GTest
    GTest::Main
)

set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER framework/${PROJECT_NAME})

file(COPY "${MEDIA_DIRECTORY}/png/rgb_3x1.png" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/bmp/kwkw_4x1_mono.bmp" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/bmp/rgb_3x1_argb.bmp" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME}_tests DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/png/rgb_3x1.png" DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/bmp/kwkw_4x1_mono.bmp" DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/bmp/rgb_3x1_argb.bmp" DESTINATION ${BIN_DIRECTORY})

add_test(${PROJECT_NAME}_tests, ${PROJECT_NAME}_tests)
