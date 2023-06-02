#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using DistanceWithVertex    = std::pair<uint64_t, uint64_t>;
using WeightedAdjecencyList = std::vector<std::vector<DistanceWithVertex>>;

auto dijkstra_algorithm(const WeightedAdjecencyList &adjacency_list,
                        uint64_t                     from,
                        uint64_t                     to) {
    constexpr uint64_t    INF = std::numeric_limits<uint64_t>::max();

    std::vector<uint64_t> min_distances(adjacency_list.size(), INF);
    min_distances[from] = 0;

    struct CustomLess {
        auto operator()(const DistanceWithVertex &left,
                        const DistanceWithVertex &right) -> bool {
            return left.first > right.first;
        }
    };

    std::priority_queue<std::pair<uint64_t, uint64_t>,
                        std::vector<std::pair<uint64_t, uint64_t>>,
                        CustomLess>
        pq{};

    pq.emplace(0, from);
    while (!pq.empty()) {
        auto [_, current] = pq.top();
        pq.pop();
        for (auto [neighbour, edge_weight] : adjacency_list[current]) {
            auto min_dist_from_current_to_neighbour =
                edge_weight + min_distances[current];

            if (min_distances[neighbour] > min_dist_from_current_to_neighbour) {
                min_distances[neighbour] = min_dist_from_current_to_neighbour;
                pq.emplace(min_dist_from_current_to_neighbour, neighbour);
            }
        }
    }
    return min_distances[to];
}

auto main() -> int {
    uint64_t order;
    uint64_t size;

    std::cin >> order >> size;

    std::vector<std::vector<DistanceWithVertex>> adjacency_list(order);

    // Вынес `from` за цикл чтобы узнать откуда начать применять алгоритм
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t from;
        uint64_t to;
        uint64_t weight;
        std::cin >> from >> to >> weight;

        adjacency_list[from].emplace_back(to, weight);
        adjacency_list[to].emplace_back(from, weight);
    }

    uint64_t from;
    uint64_t to;
    std::cin >> from >> to;

    auto res = dijkstra_algorithm(adjacency_list, from, to);

    std::cout << res << std::endl;
}
