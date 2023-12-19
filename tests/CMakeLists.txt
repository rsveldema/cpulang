cmake_minimum_required(VERSION 3.22)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)


add_executable(tester test_ddr5.cpp)
target_link_libraries(tester PUBLIC cpulang GTest::gtest_main)


include(GoogleTest)
gtest_discover_tests(tester)