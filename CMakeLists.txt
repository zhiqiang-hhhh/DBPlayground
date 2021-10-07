cmake_minimum_required(VERSION 3.20)
project(miniKV)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(contrib/gflags)
#set(gflags_DIR ${PROJECT_BINARY_DIR}/contrib/gflags)

message(STATUS "${gflags_DIR}")
add_subdirectory(contrib/glog)

add_executable(miniKV main.cpp)
target_link_libraries(miniKV miniKV_lib glog::glog)

