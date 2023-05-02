#include <cstdint>
#include <cstdlib>
#include <inttypes.h>
#include <iostream>
#include <optional>
#include <sys/types.h>

auto solve(int *array, uint64_t length) -> std::optional<uint64_t> {
    if (!length) {
        return std::nullopt;
    }
    uint64_t l_index = 0;
    uint64_t r_index = length - 1;
    while (l_index + 1 < r_index) {
        auto mid_index = l_index + ((r_index - l_index) >> 1);
        if (mid_index == l_index) {
            break;
        }
        auto mid_value = array[mid_index];
        if (array[l_index] > array[r_index] && array[r_index] <= mid_value) {
            r_index = mid_index;
        } else {
            l_index = mid_index;
        }
    }
    return array[l_index] > array[r_index] ? l_index : r_index;
}

int main() {
    uint64_t n;
    std::cin >> n;
    auto array = new int[n];
    for (uint64_t i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    auto res = solve(array, n);
    if (!res.has_value()) {
        delete[] array;
        return EXIT_FAILURE;
    }
    auto value = res.value();
    std::cout << value;

    delete[] array;
}
