#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>

auto binsearch(const int *array, int item, uint64_t l, uint64_t r) -> uint64_t {
    if (r <= l) {
        return l;
    }

    while (l <= r) {
        auto mid = l + ((r - l) >> 1);
        if (array[mid] < item) {
            l = mid + 1;
        } else if (array[mid] > item) {
            if (!mid) {
                return mid;
            }
            r = mid - 1;
        } else {
            return mid;
        }
    }
    return l;
}

int main() {
    uint64_t n, m;
    std::cin >> n >> m;
    if (!n || !m) {
        return EXIT_SUCCESS;
    }

    auto A = new int[n];
    for (uint64_t i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    uint64_t maximum_possible_index = n - 1;
    uint64_t l                      = 0;
    uint64_t r                      = 1;
    for (uint64_t i = 0; i < m; ++i) {
        int b_item;
        std::cin >> b_item;
        while (l <= maximum_possible_index && A[l] <= b_item) {
            l = r + 1;
            r = r << 1;
        }

        l        = (l) ? (l - 1) >> 1 : 0;
        r        = (r > 1) ? std::min(maximum_possible_index, (r >> 1)) : 1;
        auto res = binsearch(A, b_item, l, r);
        if (res < n && A[res] == b_item) {
            std::cout << b_item << ' ';
        }
    }

    delete[] A;
    return EXIT_SUCCESS;
}
