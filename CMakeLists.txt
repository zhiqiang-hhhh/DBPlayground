cmake_minimum_required(VERSION 3.20)
project(miniKV)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(contrib/gflags)
add_subdirectory(contrib/glog)
add_subdirectory(contrib/googletest)
add_subdirectory(src)
add_subdirectory(test)

find_package(Boost REQUIRED regex date_time system filesystem thread graph)
include_directories(${BOOST_INCLUDE_DIRS})
message("boost lib: ${Boost_LIBRARIES}")
message("boost inc:${Boost_INCLUDE_DIR}")

add_executable(miniKV main.cpp)
target_link_libraries(miniKV miniKV_lib glog::glog ${Boost_LIBRARIES})


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

