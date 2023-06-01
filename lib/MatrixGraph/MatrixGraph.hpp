#ifndef MATRIX_GRAPH_HPP
#define MATRIX_GRAPH_HPP

#include "IGraph.hpp"
#include <cstdint>
#include <vector>

class MatrixGraph : public IGraph {
 public:
    explicit MatrixGraph(uint64_t vertex_count);

    explicit MatrixGraph(const IGraph &graph);

    void               add_edge(int from, int to) override;

    [[nodiscard]] auto vertices_count() const -> int override;

    [[nodiscard]] auto get_next_vertices(int vertex) const
        -> std::vector<int> override;

    [[nodiscard]] auto get_prev_vertices(int vertex) const
        -> std::vector<int> override;

 private:
    std::vector<std::vector<bool>> adjacency_matrix_;
};

#endif  // !MATRIX_GRAPH_HPP
