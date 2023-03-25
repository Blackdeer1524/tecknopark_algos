#include "heap.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <utility>

using UserData = std::pair<uint64_t, uint64_t>;

int main() {
    auto max_heap_cmp = [](const UserData &left, const UserData &right) -> int {
        if (left.second == right.second) {
            return 0;
        } else if (left.second < right.second) {
            return 1;
        } else {
            return -1;
        }
    };
    uint64_t N, K;
    std::cin >> N >> K;
    auto maxheap = Heap<UserData>(max_heap_cmp, K);
    for (uint64_t i = 0; i < N; ++i) {
        UserData user_data;
        std::cin >> user_data.first >> user_data.second;
        maxheap.insert(std::move(user_data));
    }

    uint64_t result_buffer[K];
    uint64_t i = 0;
    for (; i < K; ++i) {
        auto res_opt = maxheap.pop_min();
        if (!res_opt.has_value()) {
            break;
        }
        result_buffer[i] = res_opt.value().first;
    }

    for (; i > 0; --i) {
        std::cout << result_buffer[i - 1] << " ";
    }
}
