#include "btree.hpp"
#include <ostream>

auto main() -> int {
    auto tree = BTree::build(2, [](int &left, int &right) -> int {
        if (left < right) {
            return -1;
        }
        if (left > right) {
            return 1;
        }
        return 0;
    });

    tree->insert(7);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(5);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(0);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(5);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(9);
    tree->print();
    std::cout << std::endl << std::endl;
}
