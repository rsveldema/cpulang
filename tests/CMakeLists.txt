cmake_minimum_required(VERSION 3.22)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)


add_executable(tester test_ddr5.cpp)
target_link_libraries(tester PUBLIC cpulang GTest::gtest_main)


include(GoogleTest)
gtest_discover_tests(tester)

add_custom_target(input1.rawbin
    COMMAND riscv64-linux-gnu-gcc -c ${CMAKE_SOURCE_DIR}/tests/input1.c
    COMMAND riscv64-linux-gnu-objcopy --dump-section .text=input1.rawbin input1.o
    COMMAND echo done
    SOURCES input1.c)


add_dependencies(tester input1.rawbin)
