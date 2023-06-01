#include "ListGraph.hpp"
#include <assert.h>
#include <queue>

ListGraph::ListGraph(int vertexCount) {
    adjLists.resize(vertexCount);
    prevAdjLists.resize(vertexCount);
}

ListGraph::ListGraph(const IGraph &graph) {
    adjLists.resize(graph.vertices_count());
    prevAdjLists.resize(graph.vertices_count());
    for (int i = 0; i < graph.vertices_count(); ++i) {
        adjLists[i]     = graph.get_next_vertices(i);
        prevAdjLists[i] = graph.get_prev_vertices(i);
    }
}

void ListGraph::add_edge(int from, int to) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());

    adjLists[from].push_back(to);
    prevAdjLists[to].push_back(from);
}

auto ListGraph::vertices_count() const -> int {
    return adjLists.size();
}

auto ListGraph::get_next_vertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
}

auto ListGraph::get_prev_vertices(int vertex) const -> std::vector<int> {
    assert(vertex >= 0 && vertex < adjLists.size());
    return prevAdjLists[vertex];
}

void bfs(const IGraph &graph, int vertex, void (*visit)(int)) {
    std::vector<bool> visited(graph.vertices_count(), false);
    std::queue<int>   bfs_q;
    bfs_q.push(vertex);
    visited[vertex] = true;
    while (!bfs_q.empty()) {
        int current = bfs_q.front();
        bfs_q.pop();
        visit(current);
        std::vector<int> adj_list = graph.get_next_vertices(current);
        for (int i : adj_list) {
            if (!visited[i]) {
                bfs_q.push(i);
                visited[i] = true;
            }
        }
    }
}
