#include <cstdint>
#include <iostream>
#include <queue>
#include <sys/types.h>
#include <vector>

auto breadth_first_search(const std::vector<std::vector<int>> &adjecency_list,
                          int                                  start,
                          int                                  end) -> int {
    auto              graph_order = adjecency_list.size();
    std::vector<int>  min_distances(graph_order, -1);
    std::vector<int>  ways(graph_order, 0);
    std::vector<bool> already_seen(graph_order, false);

    min_distances[start] = 0;
    ways[start]          = 1;

    min_distances[start] = 0;
    ways[start]          = 1;
    std::queue<int> q{};
    q.push(start);
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        if (already_seen[current]) {
            continue;
        }
        already_seen[current] = true;
        for (auto next : adjecency_list[current]) {
            if (min_distances[next] == -1) {
                min_distances[next] = min_distances[current] + 1;
                ways[next]          = ways[current];
            } else if (min_distances[next] == min_distances[current] + 1) {
                ways[next] += ways[current];
            }
            if (!already_seen[next]) {
                q.push(next);
            }
        }
    }
    return ways[end];
}

auto main() -> int {
    uint64_t v;
    uint64_t m;

    std::cin >> v >> m;

    std::vector<std::vector<int>> adjecency_list(v);

    for (uint64_t i = 0; i < m; ++i) {
        int first;
        int second;
        std::cin >> first >> second;

        adjecency_list[first].push_back(second);
        adjecency_list[second].push_back(first);
    }

    int start;
    int end;
    std::cin >> start >> end;

    auto res = breadth_first_search(adjecency_list, start, end);

    std::cout << res << std::endl;
}
