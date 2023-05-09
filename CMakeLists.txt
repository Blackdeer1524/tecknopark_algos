cmake_minimum_required(VERSION 3.22)
project(tecknopark_algos CXX)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_INIT "-Werror -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_INIT "--coverage -fno-exceptions -g -Wunused-result")
set(CMAKE_CXX_FLAGS_DEBUG_INIT
    "-fsanitize=address,undefined -fsanitize-undefined-trap-on-error")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fsanitize=address,undefined -static-libasan")

# include(CTest)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_custom_target(
    test_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action
    memcheck
    COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(lib)
add_executable(main main.cpp)
target_link_libraries(main avltree)

add_executable(submission submission.cpp)
