#ifndef LIST_GRAPH_HPP
#define LIST_GRAPH_HPP

#include "IGraph.hpp"

class ListGraph : public IGraph {
 public:
    explicit ListGraph(int vertexCount);
    explicit ListGraph(const IGraph &graph);

    void               add_edge(int from, int to) override;

    [[nodiscard]] auto vertices_count() const -> int override;

    [[nodiscard]] auto get_next_vertices(int vertex) const
        -> std::vector<int> override;

    [[nodiscard]] auto get_prev_vertices(int vertex) const
        -> std::vector<int> override;

 private:
    std::vector<std::vector<int>> adjLists;
    std::vector<std::vector<int>> prevAdjLists;
};

#endif  // !LIST_GRAPH_HPP
