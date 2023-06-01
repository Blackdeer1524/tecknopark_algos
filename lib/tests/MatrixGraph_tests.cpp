#include "ArcGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>

TEST(MatrixGraph, insertLoop) {
    auto graph = MatrixGraph(1);
    graph.add_edge(0, 0);
    std::vector<int> expected{0};
    ASSERT_EQ(expected, graph.get_next_vertices(0));
    EXPECT_EQ(expected, graph.get_prev_vertices(0));
}

TEST(MatrixGraph, insertMultipleNodes) {
    auto graph = MatrixGraph(3);
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);

    std::vector<int> expected{1, 2};
    auto             actual = graph.get_next_vertices(0);
    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(expected, actual);
}

TEST(MatrixGraph, insertMultipleNodesPrev) {
    auto graph = MatrixGraph(3);
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);

    std::vector<int> expected{0};
    auto             actual = graph.get_prev_vertices(1);
    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(expected, actual);
}

TEST(MatrixGraph, insertMultipleNodesPrev2) {
    auto graph = MatrixGraph(3);
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);

    std::vector<int> expected{0};
    auto             actual = graph.get_prev_vertices(2);
    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(expected, actual);
}

TEST(MatrixGraph, insertMultipath) {
    auto graph = MatrixGraph(1);
    graph.add_edge(0, 0);
    graph.add_edge(0, 0);
    std::vector<int> expected{0};
    EXPECT_EQ(expected, graph.get_next_vertices(0));
    EXPECT_EQ(expected, graph.get_prev_vertices(0));
}

TEST(MatrixGraph, insertWithOutOfRangeError) {
    {
        auto graph = MatrixGraph(1);
        EXPECT_DEATH({ graph.add_edge(0, 1); }, "");
    }
    {

        auto graph = MatrixGraph(1);
        EXPECT_DEATH({ graph.add_edge(-1, 0); }, "");
    }
}

TEST(MatrixCopyConstructor, ArcGraph) {
    ArcGraph left{2};
    left.add_edge(1, 0);
    left.add_edge(1, 0);
    left.add_edge(0, 0);
    left.add_edge(0, 1);

    MatrixGraph right{left};

    EXPECT_EQ(left.vertices_count(), right.vertices_count());

    std::vector<int> expected_next_for_1{0};
    std::vector<int> expected_prev_for_1{0};

    EXPECT_EQ(expected_next_for_1, right.get_next_vertices(1));
    EXPECT_EQ(expected_prev_for_1, right.get_prev_vertices(1));

    std::vector<int> expected_next_for_0{0, 1};
    std::vector<int> expected_prev_for_0{0, 1};

    auto             actual_next_for_0 = right.get_next_vertices(0);
    std::sort(actual_next_for_0.begin(), actual_next_for_0.end());

    auto actual_prev_for_0 = right.get_prev_vertices(0);
    std::sort(actual_prev_for_0.begin(), actual_prev_for_0.end());

    EXPECT_EQ(expected_next_for_0, actual_next_for_0);
    EXPECT_EQ(expected_prev_for_0, actual_prev_for_0);
}

TEST(MatrixCopyConstructor, ListGraph) {
    ListGraph left{2};
    left.add_edge(1, 0);
    left.add_edge(1, 0);
    left.add_edge(0, 0);
    left.add_edge(0, 1);

    MatrixGraph right{left};

    EXPECT_EQ(left.vertices_count(), right.vertices_count());

    std::vector<int> expected_next_for_1{0};
    std::vector<int> expected_prev_for_1{0};

    EXPECT_EQ(expected_next_for_1, right.get_next_vertices(1));
    EXPECT_EQ(expected_prev_for_1, right.get_prev_vertices(1));

    std::vector<int> expected_next_for_0{0, 1};
    std::vector<int> expected_prev_for_0{0, 1};

    auto             actual_next_for_0 = right.get_next_vertices(0);
    std::sort(actual_next_for_0.begin(), actual_next_for_0.end());

    auto actual_prev_for_0 = right.get_prev_vertices(0);
    std::sort(actual_prev_for_0.begin(), actual_prev_for_0.end());

    EXPECT_EQ(expected_next_for_0, actual_next_for_0);
    EXPECT_EQ(expected_prev_for_0, actual_prev_for_0);
}

TEST(MatrixCopyConstructor, MatrixGraph) {
    MatrixGraph left{2};
    left.add_edge(1, 0);
    left.add_edge(1, 0);
    left.add_edge(0, 0);
    left.add_edge(0, 1);

    MatrixGraph right{left};

    EXPECT_EQ(left.vertices_count(), right.vertices_count());

    std::vector<int> expected_next_for_1{0};
    std::vector<int> expected_prev_for_1{0};

    EXPECT_EQ(expected_next_for_1, right.get_next_vertices(1));
    EXPECT_EQ(expected_prev_for_1, right.get_prev_vertices(1));

    std::vector<int> expected_next_for_0{0, 1};
    std::vector<int> expected_prev_for_0{0, 1};

    auto             actual_next_for_0 = right.get_next_vertices(0);
    std::sort(actual_next_for_0.begin(), actual_next_for_0.end());

    auto actual_prev_for_0 = right.get_prev_vertices(0);
    std::sort(actual_prev_for_0.begin(), actual_prev_for_0.end());

    EXPECT_EQ(expected_next_for_0, actual_next_for_0);
    EXPECT_EQ(expected_prev_for_0, actual_prev_for_0);
}

TEST(MatrixCopyConstructor, SetGraph) {
    SetGraph left{2};
    left.add_edge(1, 0);
    left.add_edge(1, 0);
    left.add_edge(0, 0);
    left.add_edge(0, 1);

    MatrixGraph right{left};

    EXPECT_EQ(left.vertices_count(), right.vertices_count());

    std::vector<int> expected_next_for_1{0};
    std::vector<int> expected_prev_for_1{0};

    EXPECT_EQ(expected_next_for_1, right.get_next_vertices(1));
    EXPECT_EQ(expected_prev_for_1, right.get_prev_vertices(1));

    std::vector<int> expected_next_for_0{0, 1};
    std::vector<int> expected_prev_for_0{0, 1};

    auto             actual_next_for_0 = right.get_next_vertices(0);
    std::sort(actual_next_for_0.begin(), actual_next_for_0.end());

    auto actual_prev_for_0 = right.get_prev_vertices(0);
    std::sort(actual_prev_for_0.begin(), actual_prev_for_0.end());

    EXPECT_EQ(expected_next_for_0, actual_next_for_0);
    EXPECT_EQ(expected_prev_for_0, actual_prev_for_0);
}
