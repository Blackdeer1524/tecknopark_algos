#include <algorithm>
#include <cstdint>
#include <inttypes.h>
#include <iostream>
#include <optional>
#include <tuple>
#include <variant>

#define min(x, y) (((x) < (y)) ? (x) : (y))

auto binsearch(const int *array, uint64_t r, int item)
    -> std::tuple<uint64_t, uint64_t> {
    uint64_t l = 0;

    while (l <= r) {
        auto mid = l + ((r - l) >> 1);
        if (array[mid] < item) {
            l = mid + 1;
        } else if (array[mid] > item) {
            if (!mid) {
                return std::tuple(0, min(1, r));
            }
            r = mid - 1;
        } else {
            return std::tuple(mid, mid);
        }
    }
    return std::tuple(min(0, l - 2), min(l, r));
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
        int  min_value = A[min_index];
        for (uint64_t j = left + 1; j <= right; ++j) {
            if (A[j] < min_value) {
                min_value = A[j];
                min_index = j;
            }
        }
        std::cout << min_index << ' ';
    }
}
