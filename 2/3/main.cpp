#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <sys/types.h>

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
    auto A = new int[n];
    for (uint64_t i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    uint64_t l = 0;
    for (uint64_t i = 0; i < m; ++i) {
        int b_item;
        std::cin >> b_item;
        auto res = binsearch(A, b_item, l, n - 1);
        l        = res;
        if (res < n && A[res] == b_item) {
            std::cout << b_item << ' ';
            ++l;
        }
    }

    delete[] A;
}
