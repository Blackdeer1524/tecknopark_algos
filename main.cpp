#include "avltree.hpp"
#include <cstdint>
#include <iostream>
#include <sys/stat.h>

auto main() -> int {
    auto tree = AVLTree<int64_t>([](const int64_t &left, const int64_t &right) {
        if (left > right) {
            return 1;
        }
        if (left < right) {
            return -1;
        }
        return 0;
    });
    uint64_t n;
    std::cin >> n;

    for (uint64_t i = 0; i < n; ++i) {
        int64_t  value;
        uint64_t statistic;
        std::cin >> value >> statistic;
        if (value < 0) {
            tree.remove(-value);
        } else {
            tree.insert(std::move(value));
        }
        auto stat_query_res = tree.find(statistic);
        std::cout << stat_query_res.value() << ' ';
    }

    //
    // for (int i = 100; i > 0; --i) {
    //     tree.insert(std::move(i));
    // }
    // tree.insert(std::move(0));
    // tree.in_order_print();
}
