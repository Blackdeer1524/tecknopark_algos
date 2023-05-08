#include "btree.hpp"
#include <cstdint>
#include <ostream>

auto main() -> int {
    uint64_t t;
    std::cin >> t;

    auto tree =
        BTree::build(t, [](const uint32_t &left, const uint32_t &right) -> int {
            if (left < right) {
                return -1;
            }
            if (left > right) {
                return 1;
            }
            return 0;
        });
    for (;;) {
        int value;
        std::cin >> value;
        if (!std::cin) {
            break;
        }
        tree->insert(std::move(value));
    }
    tree->print();
}
