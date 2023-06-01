#include <algorithm>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <limits>
#include <memory>
#include <unordered_map>

enum class Color { WHITE, BLACK };

class Vertex {
    using neighbours_list = std::forward_list<std::shared_ptr<Vertex>>;

 public:
    explicit Vertex(int name) : name_(name) {
    }

    [[nodiscard]] constexpr auto color() const -> Color {
        return color_;
    }

    [[nodiscard]] constexpr auto name() const -> int {
        return name_;
    }

    auto neighbours() -> Vertex::neighbours_list {
        return neighbours_;
    }

    auto set_color(Color color) -> void {
        color_ = color;
    }

 private:
    int                                        name_;
    Color                                      color_{Color::WHITE};
    std::forward_list<std::shared_ptr<Vertex>> neighbours_;
};

using AdjListT = std::unordered_map<int, std::shared_ptr<Vertex>>;

auto breadth_first_search(AdjListT &&adj_list, int start, int end) -> uint64_t {
    auto adjacency_list = std::move(adj_list);
    auto start_vertex   = adjacency_list[start];
    auto vertecies = std::deque<std::pair<std::shared_ptr<Vertex>, uint64_t>>();
    uint64_t counter   = 0;
    uint64_t min_depth = std::numeric_limits<uint64_t>::max();
    vertecies.emplace_front(start_vertex, 0);
    while (!vertecies.empty()) {
        auto [vertex, depth] = vertecies.front();
        vertecies.pop_front();
        if (vertex->color() == Color::BLACK) {
            continue;
        }
        if (vertex->name() == end) {
            if (depth > min_depth) {
                break;
            }
            min_depth = depth;
            ++counter;
        } else {
            vertex->set_color(Color::BLACK);
            for (auto item : vertex->neighbours()) {
                vertecies.emplace_back(item, depth + 1);
            }
        }
    }
    return counter;
}

auto main() -> int {
    auto adjacency_list = AdjListT();

    return 0;
}
