cmake_minimum_required(VERSION 3.13)
project(DevLogicker)

#set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(run_tests)
target_include_directories(run_tests PRIVATE src)
target_include_directories(run_tests PRIVATE /usr/include)
target_link_libraries(run_tests PRIVATE gtest_main)
target_link_libraries(run_tests PRIVATE gtest)
target_link_libraries(run_tests PRIVATE pthread)
target_link_libraries(run_tests PRIVATE jsoncpp)

target_sources(run_tests PRIVATE test/catalogue/mapping_type/read_only_mapping.cpp)
target_sources(run_tests PRIVATE test/core/grid.cpp)
target_sources(run_tests PRIVATE test/grid_builder/grid_builder.cpp)
target_sources(run_tests PRIVATE test/input/json_input.cpp)
target_sources(run_tests PRIVATE test/types_util/types_util.cpp)

target_sources(run_tests PRIVATE src/input/json_input.cpp)
