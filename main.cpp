#include "hashmap.hpp"
#include <cstdint>
#include <iostream>
#include <numeric>

auto hash1(const std::string &str) -> uint64_t {
    static uint64_t a   = 13;
    uint64_t        res = 0;
    for (unsigned char c : str) {
        res = res * a + c;
    }
    return res;
}

auto hash2(const std::string &str) -> uint64_t {
    uint64_t res = 0;
    for (unsigned char c : str) {
        res = res + c;
    }
    return res;
}

auto main() -> int {
    auto map = HashMap(hash1, hash2);
    while (std::cin) {
        char op;
        std::cin >> op;
        if (!std::cin) {
            break;
        }
        std::string t;
        std::cin >> t;
        switch (op) {
            case '+': {
                auto flag = map.insert(std::move(t));
                if (flag) {
                    std::cout << "FAIL" << std::endl;
                } else {
                    std::cout << "OK" << std::endl;
                }
                // map.print();
                // std::cout << std::endl;
            } break;
            case '?': {
                auto flag = map.contains(t);
                if (flag) {
                    std::cout << "OK" << std::endl;
                } else {
                    std::cout << "FAIL" << std::endl;
                }
            } break;
            case '-': {
                auto flag = map.remove(t);
                if (flag) {
                    std::cout << "FAIL" << std::endl;
                } else {
                    std::cout << "OK" << std::endl;
                }
                // map.print();
                // std::cout << std::endl;
            } break;
            default:
                exit(0);
                break;
        }
    }
}
