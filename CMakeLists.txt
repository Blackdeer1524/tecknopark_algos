cmake_minimum_required(VERSION 3.22)
project(tecknopark_algos CXX)
set(CMAKE_CXX_STANDARD 20)

set(CUR_MODULE CACHE STRING "module")
set(QUESTION CACHE STRING "question")

include(CTest)

add_custom_target(
  test_memcheck
  COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action
          memcheck
  COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory("./${MODULE}/${QUESTION}")
