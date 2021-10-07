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


add_executable(miniKV main.cpp)
target_link_libraries(miniKV miniKV_lib glog::glog)

