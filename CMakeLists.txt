cmake_minimum_required(VERSION 3.22)
project(tecknopark_algos CXX)
set(CMAKE_CXX_STANDARD 20)

include(CTest)

set(CMAKE_CXX_FLAGS_INIT "-Werror -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_INIT "--coverage -fno-exceptions -g -Wunused-result")

set(CMAKE_CXX_FLAGS_DEBUG_INIT
    "-fsanitize=address,undefined -fsanitize-undefined-trap-on-error")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fsanitize=address,undefined -static-libasan")

add_custom_target(
    test_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action
    memcheck
    COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(statistics main.cpp)
