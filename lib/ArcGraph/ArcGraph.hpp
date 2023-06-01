#ifndef ARC_GRAPH_HPP
#define ARC_GRAPH_HPP

#include "IGraph.hpp"
#include <cstdint>
#include <vector>

class ArcGraph : public IGraph {
 public:
    explicit ArcGraph(uint64_t graph_order);

    explicit ArcGraph(const IGraph &graph);

    void               add_edge(int from, int to) override;

    [[nodiscard]] auto vertices_count() const -> int override;

    [[nodiscard]] auto get_next_vertices(int vertex) const
        -> std::vector<int> override;

    [[nodiscard]] auto get_prev_vertices(int vertex) const
        -> std::vector<int> override;

 private:
    std::vector<std::pair<int, int>> arcs_;
    uint64_t                         graph_order_;
};

#endif  // !SET_GRAPH_HPP
