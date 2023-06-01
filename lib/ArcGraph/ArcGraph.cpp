#include "ArcGraph.hpp"
#include "IGraph.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <vector>

ArcGraph::ArcGraph(uint64_t graph_order) : graph_order_(graph_order) {
}

ArcGraph::ArcGraph(const IGraph &graph) : graph_order_(graph.vertices_count()) {
    for (auto cur_vert : std::views::iota(0, graph.vertices_count())) {
        for (auto next_vert : graph.get_next_vertices(cur_vert)) {
            arcs_.emplace_back(cur_vert, next_vert);
        }
    }
}

auto ArcGraph::add_edge(int from, int to) -> void {
    assert(0 <= from && from < graph_order_);
    assert(0 <= to && to < graph_order_);

    arcs_.emplace_back(from, to);
}

auto ArcGraph::vertices_count() const -> int {
    return graph_order_;
}

auto ArcGraph::get_next_vertices(int vertex) const -> std::vector<int> {
    auto res = std::vector<int>();
    auto vertecies =
        std::views::filter(
            arcs_,
            [vertex](const auto &item) { return item.first == vertex; }) |
        std::views::transform([](const auto &item) { return item.second; });
    std::copy(vertecies.begin(), vertecies.end(), std::back_inserter(res));
    return res;
}

auto ArcGraph::get_prev_vertices(int vertex) const -> std::vector<int> {
    auto res = std::vector<int>();
    auto vertecies =
        std::views::filter(
            arcs_,
            [vertex](const auto &item) { return item.second == vertex; }) |
        std::views::transform([](const auto &item) { return item.first; });
    std::copy(vertecies.begin(), vertecies.end(), std::back_inserter(res));
    return res;
}
