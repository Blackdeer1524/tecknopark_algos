#include <cstdint>
#include <inttypes.h>
#include <iostream>
#include <random>
#include <sys/types.h>
#include <utility>

using std::pair;

template <typename T>
auto partition(T       *array,
               uint64_t l,
               uint64_t r,
               auto(*cmp)(const T &left, const T &right)->int) -> uint64_t {
    auto     pivot = array[r];
    uint64_t j     = l - 1;
    for (uint64_t i = l; i <= r - 1; ++i) {
        if (cmp(array[i], pivot) <= 0) {
            std::swap(array[++j], array[i]);
        }
    }
    std::swap(array[++j], array[r]);
    return j;
}

template <typename T>
auto random_partition(T       *array,
                      uint64_t l,
                      uint64_t r,
                      auto(*cmp)(const T &left, const T &right)->int)
    -> uint64_t {
    auto     subarray_length    = r - l + 1;
    uint64_t random_pivot_index = rand() % subarray_length;
    std::swap(array[r], array[l + random_pivot_index]);
    return partition(array, l, r, cmp);
}

template <typename T>
auto quick_sort(T       *array,
                uint64_t l,
                uint64_t r,
                auto(*cmp)(const T &left, const T &right)->int) -> void {
    if (l >= r) {
        return;
    }
    auto q = random_partition(array, l, r, cmp);
    if (q) {
        quick_sort(array, l, q - 1, cmp);
    }
    quick_sort(array, q + 1, r, cmp);
}

auto main() -> int {
    uint64_t N;

    std::cin >> N;
    N <<= 1;

    auto array = new pair<int, int>[N];
    for (uint64_t i = 0; i < N; i += 2) {
        int first, second;
        std::cin >> first >> second;
        array[i]     = std::make_pair(first, 1);
        array[i + 1] = std::make_pair(second, -1);
    }

    auto cmp = [](const pair<int, int> &l, const pair<int, int> &r) -> int {
        if (l.first < r.first) {
            return -1;
        } else if (l.first > r.first) {
            return 1;
        }
        return 0;
    };

    quick_sort<pair<int, int>>(array, 0, N - 1, cmp);
    auto    segment_start = array[0].first;  // always positive
    int64_t coloring_coef = 1;
    int64_t result        = 0;
    for (uint64_t i = 1; i < N; ++i) {
        auto    current_segment_point = array[i];
        int64_t length = current_segment_point.first - segment_start;
        if (coloring_coef == 1) {
            result += length;
        }
        coloring_coef += current_segment_point.second;
        segment_start = current_segment_point.first;
    }
    std::cout << result;

    delete[] array;
}
