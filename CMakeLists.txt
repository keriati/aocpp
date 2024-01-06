cmake_minimum_required(VERSION 3.27)
project(aocpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

add_executable(AoC2023d16 2023/16/day16.cpp)
target_compile_options(AoC2023d16 PRIVATE -O3)
