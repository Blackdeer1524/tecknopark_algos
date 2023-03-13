#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>

auto binsearch(const int *array, uint64_t length, int64_t query)
    -> std::optional<uint64_t> {
    if (!length) {
        return std::nullopt;
    }
    uint64_t left  = 0;
    uint64_t right = length - 1;
    while (left <= right) {
        auto mid     = left + ((right - left) >> 1);
        auto current = array[mid];
        if (current < query) {
            left = mid + 1;
        } else if (current > query) {
            if (!mid) {
                return 0;
            }
            right = mid - 1;
        } else {
            return mid;
        }
    }
    return left;
}

int main() {
    uint64_t n, m;
    std::cin >> n >> m;
    if (!n) {
        return EXIT_SUCCESS;
    }

    auto A = new int[n];
    for (uint64_t i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    for (uint64_t i = 0; i < m; ++i) {
        uint64_t b_item;
        std::cin >> b_item;
        auto res_option = binsearch(A, n, b_item);
        if (!res_option.has_value()) {
            delete[] A;
            return EXIT_FAILURE;
        }
        auto res_index = res_option.value();

        std::cout << res_index << ' ';
    }

    delete[] A;
}
