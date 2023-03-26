#include "heap.hpp"
#include <assert.h>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sys/types.h>
#include <utility>

auto min_heap_cmp(const int &left, const int &right) -> int {
    if (left == right) {
        return 0;
    } else if (left < right) {
        return -1;
    } else {
        return 1;
    }
};

auto test_algorithm() -> void {
    int  l = -100, r = 1000;
    int  K               = 100;
    int  interval_length = r - l + 1;
    auto heap            = Heap<int>(min_heap_cmp);
    for (int c = 0; c < K; ++c) {
        heap.push(std::move(l + c));
    }
    auto heap_min = heap.get_min().value();
    for (int i = l + K; i < r; ++i) {
        if (i > heap_min) {
            heap.pop();
            heap_min = i;
            heap.push(std::move(i));
        }
    }
    int expectation = r - K;
    for (int i = 0; i < K; ++i) {
        auto res = heap.pop().value();
        if (res != expectation) {
            std::cout << expectation << ' ' << res;
            exit(1);
        }
        ++expectation;
    }
}

auto test_heapification() -> void {
    int  l = -100, r = 1000;
    int  interval_length = r - l + 1;
    auto heap            = Heap<int>(min_heap_cmp);
    for (int i = l; i <= r; ++i) {
        heap.push(std::move(i));
    }
    auto expected = l;
    auto cmp_item = heap.pop().value();
    assert(expected == cmp_item);
    for (uint64_t i = 1; i < interval_length; ++i) {
        expected += 1;
        auto t       = heap.pop().value();
        auto cmp_res = min_heap_cmp(cmp_item, t);
        if (cmp_res > 0) {
            std::cout << cmp_item << " " << t;
            exit(-1);
        }
        cmp_item = t;
        if (expected != cmp_item) {
            std::cout << expected << " " << cmp_item;
            exit(-1);
        }
    }
    std::cout << cmp_item;
}

int main() {
    test_heapification();
    test_algorithm();
}
