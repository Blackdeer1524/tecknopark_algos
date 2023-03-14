#include <cstdint>
#include <inttypes.h>
#include <iostream>
#include <optional>
#include <tuple>
#include <variant>

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define abs(x)    ((x) < 0 ? -(x) : (x))

auto binsearch(const int *array, uint64_t length, int item)
    -> std::tuple<uint64_t, uint64_t> {
    auto     r = length;
    uint64_t l = 0;

    while (l <= r) {
        auto mid = l + ((r - l) >> 1);
        if (array[mid] < item) {
            l = mid + 1;
        } else if (array[mid] > item) {
            if (!mid) {
                return std::tuple(0, min(1, length));
            }
            r = mid - 1;
        } else {
            return std::tuple(mid, mid);
        }
    }
    // min(l, l - 1) стоит против underflow
    return std::tuple(min(l, l - 1), min(l, length));
}

int main() {
    uint64_t n;
    std::cin >> n;
    auto A = new int[n];
    for (uint64_t i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    uint64_t m;
    std::cin >> m;
    for (uint64_t i = 0; i < m; ++i) {
        int b_item;
        std::cin >> b_item;
        auto res       = binsearch(A, n - 1, b_item);
        auto left      = std::get<0>(res);
        auto right     = std::get<1>(res);
        auto min_index = left;
        int  min_value = abs(b_item - A[min_index]);
        for (uint64_t j = left + 1; j <= right; ++j) {
            auto difference = abs(A[j] - b_item);
            if (difference < min_value) {
                min_value = difference;
                min_index = j;
            }
        }
        std::cout << min_index << ' ';
    }
}
