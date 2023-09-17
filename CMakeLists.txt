cmake_minimum_required(VERSION 3.20)
project(miniKV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MiniKV_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

if(APPLE)
    # 检查操作系统是否为 macOS
    execute_process(
        COMMAND uname -m
        OUTPUT_VARIABLE ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(ARCH STREQUAL "arm64")
        # 设置ARCH_AARCH64为True
        set(ARCH_AARCH64 True)
    endif()
    # 设置OS_DARWIN为True
    set(OS_DARWIN True)
endif()

message(STATUS "ARCH_AARCH64: ${ARCH_AARCH64}")
message(STATUS "OS_DARWIN: ${OS_DARWIN}")

add_subdirectory(contrib/glog-cmake)
add_subdirectory(contrib/zlib-ng-cmake)
add_subdirectory(contrib/boost-cmake)
add_subdirectory(contrib/googletest-cmake)

add_subdirectory(src)

add_executable(miniKV)
target_sources(miniKV PRIVATE main.cpp)
target_link_libraries(miniKV PRIVATE miniKV_lib)

######################################################################################################################
# Make Target
######################################################################################################################

option(MNKV_BUILD_TEST "MiniKV build test" ON)
message(STATUS "MiniKV build test: " ${MNKV_BUILD_TEST})
if(${MNKV_BUILD_TEST})
    enable_testing()
    include(CTest)
    add_subdirectory(test)
endif()

# clang-format
find_program(CLANG_FORMAT_BIN
        NAMES clang-format clang-format-8
        HINTS ${CLANG_SEARCH_PATH})
if ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    message(WARNING "Couldn't find clang-format.")
else()
    message(STATUS "Found clang-format at ${CLANG_FORMAT_BIN}")
endif()

string(CONCAT MINIKV_FORMAT_DIRS
        "${PROJECT_SOURCE_DIR}/src,"
        "${PROJECT_SOURCE_DIR}/test"
        )

set(BUILD_SUPPORT_DIR "${PROJECT_SOURCE_DIR}/build-support")

# runs clang format and updates files in place.
add_custom_target(
        format 
        ${BUILD_SUPPORT_DIR}/run_clang_format.py ${CLANG_FORMAT_BIN}
        ${BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs ${MINIKV_FORMAT_DIRS} --fix --quiet)

