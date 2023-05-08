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

    // tree->insert(7);
    // tree->print();
    // std::cout << std::endl << std::endl;

    // tree->insert(5);
    // tree->print();
    // std::cout << std::endl << std::endl;

    // tree->insert(0);
    // tree->print();
    // std::cout << std::endl << std::endl;

    // tree->insert(5);
    // tree->print();
    // std::cout << std::endl << std::endl;

    // tree->insert(9);
    // tree->insert(9);
    // tree->insert(9);
    // tree->insert(9);
    // tree->print();
    // std::cout << std::endl << std::endl;

    // 2
    // 0 1 2 3 4 5 6 7 8 9

    tree->insert(0);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(1);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(2);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(3);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(4);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(5);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(6);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(7);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(8);
    tree->print();
    std::cout << std::endl << std::endl;

    tree->insert(9);
    tree->print();
    std::cout << std::endl << std::endl; }
