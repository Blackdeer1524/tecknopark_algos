#ifndef SET_GRAPH_HPP
#define SET_GRAPH_HPP

#include "IGraph.hpp"
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class SetGraph : public IGraph {
 public:
    explicit SetGraph(uint64_t vertex_count);

    explicit SetGraph(const IGraph &graph);

    void               add_edge(int from, int to) override;

    [[nodiscard]] auto vertices_count() const -> int override;

    [[nodiscard]] auto get_next_vertices(int vertex) const
        -> std::vector<int> override;

    [[nodiscard]] auto get_prev_vertices(int vertex) const
        -> std::vector<int> override;

 private:
    std::vector<std::unordered_set<int>> next_vertecies_;
    std::vector<std::unordered_set<int>> prev_vertecies_;
};

#endif  // !SET_GRAPH_HPP
