cmake_minimum_required(VERSION 3.27)
project(aocpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable(AoC2023d16 2023/16/day16.cpp)
add_executable(AoC2023d17 2023/17/day17.cpp)
add_executable(AoC2024d01 2024/01/day01.cpp)
add_executable(mergeSort mergeSort.cpp)
add_executable(getkey getkey.cpp)