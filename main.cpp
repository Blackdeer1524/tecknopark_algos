#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

class Vertex {
 public:
    static auto build() -> Vertex * {
        auto *node    = new Vertex();
        node->parent_ = node;
        return node;
    }

    auto unite(Vertex *other) -> void {
        link(find(), other->find());
    }

    auto find() -> Vertex * {
        if (this != parent_) {
            parent_ = parent_->find();
        }
        return parent_;
    }

 private:
    Vertex() = default;

    uint64_t    rank_{};
    Vertex     *parent_;

    static auto link(Vertex *x, Vertex *y) -> void {
        if (x->rank_ > y->rank_) {
            y->parent_ = x;
            return;
        }
        x->parent_ = y;
        if (x->rank_ == y->rank_) {
            ++y->rank_;
        }
    }
};

struct Edge {
    Edge(uint64_t weight, Vertex *start, Vertex *end)
        : weight_(weight), start_(start), end_(end) {
    }

    uint64_t weight_;
    Vertex  *start_;
    Vertex  *end_;
};

auto main() -> int {
    uint64_t order;
    uint64_t size;
    std::cin >> order >> size;

    auto              vertecies = std::vector<Vertex *>(order, nullptr);
    std::vector<Edge> edges;
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t start;
        uint64_t end;
        uint64_t weight;

        std::cin >> start >> end >> weight;
        --start;
        --end;
        auto &start_vertex = vertecies[start];
        if (start_vertex == nullptr) {
            start_vertex = Vertex::build();
        }

        auto &end_vertex = vertecies[end];
        if (end_vertex == nullptr) {
            end_vertex = Vertex::build();
        }

        edges.emplace_back(weight, start_vertex, end_vertex);
    }
    std::sort(edges.begin(),
              edges.end(),
              [](const Edge &left, const Edge &right) -> int {
                  return left.weight_ < right.weight_;
              });

    uint64_t res = 0;
    for (auto &edge : edges) {
        if (edge.start_->find() != edge.end_->find()) {
            edge.start_->unite(edge.end_);
            res += edge.weight_;
        }
    }
    std::cout << res << std::endl;

    for (auto *vertex : vertecies) {
        delete vertex;
    }
}
