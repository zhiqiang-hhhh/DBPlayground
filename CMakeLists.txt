cmake_minimum_required(VERSION 3.20)
project(miniKV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MiniKV_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR})

cmake_policy(SET CMP0077 NEW)
set(WITH_GFLAGS OFF)
set(WITH_GTEST OFF)
set(WITH_PKGCONFIG OFF)
add_subdirectory(contrib/glog)
add_subdirectory(contrib/zlib-ng-cmake)
add_subdirectory(contrib/boost-cmake)
add_subdirectory(contrib/googletest-cmake)

add_subdirectory(src)
add_subdirectory(test)

add_executable(miniKV)
target_sources(miniKV PRIVATE main.cpp)
target_link_libraries(miniKV PRIVATE miniKV_lib)


######################################################################################################################
# Make Target
######################################################################################################################

# clang-format
find_program(CLANG_FORMAT_BIN
        NAMES clang-format clang-format-8
        HINTS ${CLANG_SEARCH_PATH})
if ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    message(WARNING "couldn't find clang-format.")
else()
    message(STATUS "found clang-format at ${CLANG_FORMAT_BIN}")
endif()

string(CONCAT MINIKV_FORMAT_DIRS
        "${PROJECT_SOURCE_DIR}/src,"
        "${PROJECT_SOURCE_DIR}/test"
        )

set(BUILD_SUPPORT_DIR "${PROJECT_SOURCE_DIR}/build-support")

# runs clang format and updates files in place.
add_custom_target(format
        ${BUILD_SUPPORT_DIR}/run_clang_format.py
        ${CLANG_FORMAT_BIN}
        ${BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs
        ${MINIKV_FORMAT_DIRS}
        --fix
        --quiet
        )

