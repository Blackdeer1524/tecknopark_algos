
add_executable(
    avl_tests
    tests.cpp
)
target_link_libraries(
    avl_tests
    avltree
    GTest::gtest_main
)

gtest_discover_tests(avl_tests)
