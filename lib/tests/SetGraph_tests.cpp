#include "SetGraph.hpp"
#include "gtest/gtest.h"
#include <algorithm>
#include <vector>

namespace {
auto is_permutation(const std::vector<int> &vec1, const std::vector<int> &vec2)
    -> bool {
    if (vec1.size() != vec2.size()) {
        return false;
    }

    std::vector<int> sorted_vec1 = vec1;
    std::vector<int> sorted_vec2 = vec2;

    std::sort(sorted_vec1.begin(), sorted_vec1.end());
    std::sort(sorted_vec2.begin(), sorted_vec2.end());

    return sorted_vec1 == sorted_vec2;
}
}  // namespace

TEST(SetGraph, insertLoop) {
    auto graph = SetGraph(1);
    graph.add_edge(0, 0);
    std::vector<int> expected{0};
    ASSERT_EQ(expected, graph.get_next_vertices(0));
    EXPECT_EQ(expected, graph.get_prev_vertices(0));
}

TEST(SetGraph, insertMultipleNodes) {
    auto graph = SetGraph(3);
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);

    std::vector<int> expected{1, 2};
    auto             actual = graph.get_next_vertices(0);
    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(expected, actual);
}

TEST(SetGraph, insertMultipleNodesPrev) {
    auto graph = SetGraph(3);
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);

    std::vector<int> expected{0};
    auto             actual = graph.get_prev_vertices(1);
    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(expected, actual);
}

TEST(SetGraph, insertMultipleNodesPrev2) {
    auto graph = SetGraph(3);
    graph.add_edge(0, 1);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);

    std::vector<int> expected{0};
    auto             actual = graph.get_prev_vertices(2);
    std::sort(actual.begin(), actual.end());

    ASSERT_EQ(expected, actual);
}

TEST(SetGraph, insertMultipath) {
    auto graph = SetGraph(1);
    graph.add_edge(0, 0);
    graph.add_edge(0, 0);
    std::vector<int> expected{0};
    EXPECT_EQ(expected, graph.get_next_vertices(0));
    EXPECT_EQ(expected, graph.get_prev_vertices(0));
}

TEST(SetGraph, insertWithOutOfRangeError) {
    {
        auto graph = SetGraph(1);
        EXPECT_DEATH({ graph.add_edge(0, 1); }, "");
    }
    {

        auto graph = SetGraph(1);
        EXPECT_DEATH({ graph.add_edge(-1, 0); }, "");
    }
}
