#include "btree.hpp"
#include <cstdint>
#include <ostream>

auto main() -> int {
    uint64_t t;
    std::cin >> t;

    auto tree = BTree::build(t, [](int &left, int &right) -> int {
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
