add_subdirectory(lib)
add_executable(merge_arrays main.cpp)
target_link_libraries(merge_arrays heap)

add_subdirectory(tests)
