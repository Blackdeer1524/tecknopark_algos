#include "MatrixGraph.hpp"
#include "IGraph.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <vector>

MatrixGraph::MatrixGraph(uint64_t vertex_count) {
    adjacency_matrix_.resize(vertex_count);
    for (auto &row : adjacency_matrix_) {
        row.resize(vertex_count);
    }
}

MatrixGraph::MatrixGraph(const IGraph &graph)
    : MatrixGraph(graph.vertices_count()) {
    for (int vertex = 0; vertex < graph.vertices_count(); ++vertex) {
        for (auto next_vertex : graph.get_next_vertices(vertex)) {
            add_edge(vertex, next_vertex);
        }
    }
}

auto MatrixGraph::add_edge(int from, int to) -> void {
    assert(0 <= from && from < vertices_count());
    assert(0 <= to && to < vertices_count());

    adjacency_matrix_[from][to] = true;
    adjacency_matrix_[to][from] = true;
}

auto MatrixGraph::vertices_count() const -> int {
    return adjacency_matrix_.size();
}

auto MatrixGraph::get_next_vertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0 && vertex < vertices_count());

    auto res = std::vector<int>();
    for (int i = 0; i < adjacency_matrix_.size(); ++i) {
        if (adjacency_matrix_[vertex][i]) {
            res.push_back(i);
        }
    }
    return res;
}

auto MatrixGraph::get_prev_vertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0 && vertex < vertices_count());

    auto res = std::vector<int>();
    for (int i = 0; i < adjacency_matrix_.size(); ++i) {
        if (adjacency_matrix_[i][vertex]) {
            res.push_back(i);
        }
    }
    return res;
}
