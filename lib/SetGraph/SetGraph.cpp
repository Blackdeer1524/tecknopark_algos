#include "SetGraph.hpp"
#include "IGraph.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <vector>

SetGraph::SetGraph(uint64_t vertex_count)
    : next_vertecies_(vertex_count), prev_vertecies_(vertex_count) {
}

SetGraph::SetGraph(const IGraph &graph) : SetGraph(graph.vertices_count()) {
    for (int cur_vert = 0; cur_vert < graph.vertices_count(); ++cur_vert) {
        for (int next_vert : graph.get_next_vertices(cur_vert)) {
            add_edge(cur_vert, next_vert);
        }
    }
}

auto SetGraph::add_edge(int from, int to) -> void {
    assert(from >= 0);
    assert(to >= 0);

    auto max_dim = std::max(from, to);
    if (next_vertecies_.size() <= max_dim) {
        next_vertecies_.resize(max_dim + 1);
        prev_vertecies_.resize(max_dim + 1);
    }
    next_vertecies_[from].insert(to);
    prev_vertecies_[to].insert(from);
}

auto SetGraph::vertices_count() const -> int {
    return next_vertecies_.size();
}

auto SetGraph::get_next_vertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0 && vertex < vertices_count());

    std::vector<int> res{};
    const auto      &vertecies = next_vertecies_[vertex];
    std::copy(vertecies.begin(), vertecies.end(), std::back_inserter(res));
    return res;
}

auto SetGraph::get_prev_vertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0 && vertex < vertices_count());

    std::vector<int> res{};
    const auto      &vertecies = prev_vertecies_[vertex];
    std::copy(vertecies.begin(), vertecies.end(), std::back_inserter(res));
    return res;
}
