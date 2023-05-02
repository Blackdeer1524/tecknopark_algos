#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <utility>

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
auto get_nth_order_statistic(T       *array,
                             uint64_t length,
                             uint64_t order_statistic,  // индексируется с 0
                             auto(*cmp)(const T &left, const T &right)->int)
    -> T {
    uint64_t l = 0;
    uint64_t r = length - 1;
    while (l < r) {
        auto q = random_partition(array, l, r, cmp);
        auto k = q - l;  // индекс пивота в массиве array[l..r]
        if (order_statistic == k) {
            return array[q];
        } else if (order_statistic < k) {
            r = q - 1;
        } else {
            order_statistic -= k + 1;
            l = q + 1;
        }
    }
    return array[l];
}

auto main() -> int {
    auto cmp = [](const int &left, const int &right) -> int {
        if (left == right) {
            return 0;
        } else if (left < right) {
            return -1;
        } else {
            return 1;
        }
    };
    uint64_t length;
    std::cin >> length;
    if (!length) {
        return 0;
    }
    int *array = new int[length];
    for (uint64_t i = 0; i < length; i++) {
        std::cin >> array[i];
    }
    uint64_t tenth     = length / 10;
    uint64_t ninetieth = 9 * length / 10;
    uint64_t median    = length / 2;
    std::cout << get_nth_order_statistic<int>(array, length, tenth, cmp)
              << std::endl
              << get_nth_order_statistic<int>(array, length, median, cmp)
              << std::endl
              << get_nth_order_statistic<int>(array, length, ninetieth, cmp);
    delete[] array;
}
