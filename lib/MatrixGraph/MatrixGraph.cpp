#include "MatrixGraph.hpp"
#include "IGraph.hpp"
#include <algorithm>
#include <cassert>
#include <iterator>
#include <ranges>
#include <vector>

auto MatrixGraph::add_edge(int from, int to) -> void {
    assert(from >= 0);
    assert(to >= 0);

    auto max_dim = std::max(from, to);
    if (adjacency_matrix_.size() <= max_dim) {
        for (auto &row : adjacency_matrix_) {
            row.resize(max_dim + 1);
        }
        adjacency_matrix_.resize(max_dim + 1);
    }
    adjacency_matrix_[from][to] = true;
}

auto MatrixGraph::vertices_count() const -> int {
    return adjacency_matrix_.size();
}

auto MatrixGraph::get_next_vertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0);

    auto res = std::vector<int>();
    for (int i = 0; i < adjacency_matrix_.size(); ++i) {
        if (adjacency_matrix_[vertex][i]) {
            res.push_back(i);
        }
    }
    return res;
}

auto MatrixGraph::get_prev_vertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0);

    auto res = std::vector<int>();
    for (int i = 0; i < adjacency_matrix_.size(); ++i) {
        if (adjacency_matrix_[i][vertex]) {
            res.push_back(i);
        }
    }
    return res;
}

MatrixGraph::MatrixGraph(const IGraph &graph) {
    for (int vertex = 0; vertex < graph.vertices_count(); ++vertex) {
        for (auto next_vertex : graph.get_next_vertices(vertex)) {
            add_edge(vertex, next_vertex);
        }
    }
}
