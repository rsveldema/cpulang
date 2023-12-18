cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

project(cpulang CXX)

add_executable(tester test.cpp)

target_include_directories(tester PUBLIC examples/blocks inc)
