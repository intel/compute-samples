#
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

if(NOT TARGET ze_loader)
    find_path(LevelZero_INCLUDE_DIR
      NAMES ze_api.h
      PATHS
      ${L0_ROOT}
      PATH_SUFFIXES "include" "include/level_zero"
      NO_DEFAULT_PATH
    )

    find_library(LevelZero_LIBRARY
      NAMES ze_loader ze_loader32 ze_loader64
      PATHS
      ${L0_ROOT}
      PATH_SUFFIXES "lib" "lib/level_zero/"
      NO_DEFAULT_PATH
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(LevelZero
      REQUIRED_VARS
        LevelZero_INCLUDE_DIR
        LevelZero_LIBRARY
      HANDLE_COMPONENTS
    )
    mark_as_advanced(LevelZero_LIBRARY LevelZero_INCLUDE_DIR)

    if(LevelZero_FOUND)
        list(APPEND LevelZero_LIBRARIES ${LevelZero_LIBRARY} ${CMAKE_DL_LIBS})
        list(APPEND LevelZero_INCLUDE_DIRS ${LevelZero_INCLUDE_DIR})
        if(OpenCL_FOUND)
            list(APPEND LevelZero_INCLUDE_DIRS ${OpenCL_INCLUDE_DIRS})
        endif()
    endif()

    add_library(ze_loader INTERFACE IMPORTED)
    set_target_properties(ze_loader
      PROPERTIES INTERFACE_LINK_LIBRARIES "${LevelZero_LIBRARIES}"
    )
    set_target_properties(ze_loader
      PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LevelZero_INCLUDE_DIRS}"
    )
endif()
