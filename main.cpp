#include "btree.hpp"
#include <algorithm>
#include <cstdint>
#include <ostream>

auto main() -> int {
    uint64_t t;
    std::cin >> t;
    // uint64_t t    = 2;

    auto tree = BTree<uint64_t>::build(
        t, [](const uint32_t &left, const uint32_t &right) -> int {
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
    // tree->insert(0);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(1);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(2);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(3);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(4);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(5);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(6);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(7);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(8);
    // tree->print();
    // std::cout << std::endl << std::endl;
    //
    // tree->insert(9);
    // tree->print();
}
