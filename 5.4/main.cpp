#include <inttypes.h>
#include <iostream>
#include <utility>

using std::pair;

template <typename T>
auto merge(T *array, uint64_t l, uint64_t q, uint64_t r,
           auto(*cmp)(const T &left, const T &right)->int) {
    if (l >= r || q > r) {
        return;
    }
    uint64_t left_index        = 0;
    auto     left_part_length  = q - l + 1;
    auto     left_part         = new T[left_part_length];

    uint64_t right_index       = 0;
    auto     right_part_length = r - q;
    auto     right_part        = new T[right_part_length];

    for (uint64_t i = l; i <= q; ++i) {
        left_part[i - l] = std::move(array[i]);
    }
    for (uint64_t i = q + 1; i <= r; ++i) {
        right_part[i - q - 1] = std::move(array[i]);
    }

    while (left_index < left_part_length && right_index < right_part_length) {
        if (cmp(left_part[left_index], right_part[right_index]) < 0) {
            array[l++] = std::move(left_part[left_index++]);
        } else {
            array[l++] = std::move(right_part[right_index++]);
        }
    }
    while (left_index < left_part_length) {
        array[l++] = left_part[left_index++];
    }
    while (right_index < right_part_length) {
        array[l++] = right_part[right_index++];
    }

    delete[] left_part;
    delete[] right_part;
}

template <typename T>
auto merge_sort(T *array, uint64_t l, uint64_t r,
                auto(*cmp)(const T &left, const T &right)->int) -> void {
    if (l >= r) {
        return;
    }
    auto q = l + ((r - l) >> 1);
    merge_sort(array, l, q, cmp);
    merge_sort(array, q + 1, r, cmp);
    merge(array, l, q, r, cmp);
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

    merge_sort<pair<int, int>>(array, 0, N - 1, cmp);
    auto    segment_start = array[0].first;  // always positive
    int64_t coloring_coef = 1;
    int64_t result        = 0;
    for (uint64_t i = 1; i < N; ++i) {
        auto current_segment_point = array[i];
        if (coloring_coef == 1) {
            int64_t length = current_segment_point.first - segment_start;
            result += length;
        }
        coloring_coef += current_segment_point.second;
        segment_start = current_segment_point.first;
    }
    std::cout << result;

    delete[] array;
}
