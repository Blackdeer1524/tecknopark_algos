#include "heap.hpp"
#include <cstdint>
#include <iostream>
#include <utility>

using UserData = std::pair<uint64_t, uint64_t>;

int main() {
    auto min_heap_cmp = [](const UserData &left, const UserData &right) -> int {
        if (left.second == right.second) {
            return 0;
        } else if (left.second < right.second) {
            return -1;
        } else {
            return 1;
        }
    };
    uint64_t N, K;
    std::cin >> N >> K;
    if (N < K) {
        return -1;
    }
    auto maxheap = Heap<UserData>(min_heap_cmp);
    for (uint64_t i = 0; i < K; ++i) {
        UserData user_data;
        std::cin >> user_data.first >> user_data.second;
        maxheap.push(std::move(user_data));
    }

    auto heap_min = maxheap.get_min().value().second;
    for (uint64_t i = K; i < N; ++i) {
        UserData user_data;
        std::cin >> user_data.first >> user_data.second;
        if (heap_min < user_data.second) {
            maxheap.push_pop(std::move(user_data));
            heap_min = maxheap.get_min().value().second;
        }
    }

    for (uint64_t i = 0; i < K; ++i) {
        auto res_opt = maxheap.pop();
        std::cout << res_opt->first << ' ';
    }
}
