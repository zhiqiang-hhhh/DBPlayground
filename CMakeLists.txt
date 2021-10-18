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

