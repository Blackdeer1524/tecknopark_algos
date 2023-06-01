#ifndef IGRAPH_HPP
#define IGRAPH_HPP

#include <vector>

struct IGraph {
    virtual ~IGraph()                                        = default;

    // Добавление ребра от from к to.
    virtual void               add_edge(int from, int to)    = 0;

    [[nodiscard]] virtual auto vertices_count() const -> int = 0;

    [[nodiscard]] virtual auto get_next_vertices(int vertex) const
        -> std::vector<int> = 0;

    [[nodiscard]] virtual auto get_prev_vertices(int vertex) const
        -> std::vector<int> = 0;
};

#endif  // !IGRAPH_HPP
