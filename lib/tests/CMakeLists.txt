add_executable(
    ArcGraph_tests
    ArcGraph_tests.cpp
)
target_link_libraries(
    ArcGraph_tests
    ArcGraph
    ListGraph
    MatrixGraph
    SetGraph
    GTest::gtest_main
)

gtest_discover_tests(ArcGraph_tests)


add_executable(
    ListGraph_tests
    ListGraph_tests.cpp
)
target_link_libraries(
    ListGraph_tests
    ArcGraph
    ListGraph
    MatrixGraph
    SetGraph
    GTest::gtest_main
)

gtest_discover_tests(ListGraph_tests)

add_executable(
    MatrixGraph_tests
    MatrixGraph_tests.cpp
)
target_link_libraries(
    MatrixGraph_tests
    ArcGraph
    ListGraph
    MatrixGraph
    SetGraph
    GTest::gtest_main
)

gtest_discover_tests(MatrixGraph_tests)


add_executable(
    SetGraph_tests
    SetGraph_tests.cpp
)
target_link_libraries(
    SetGraph_tests
    ArcGraph
    ListGraph
    MatrixGraph
    SetGraph
    GTest::gtest_main
)

gtest_discover_tests(SetGraph_tests)
