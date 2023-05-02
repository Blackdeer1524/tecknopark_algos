#include <iostream>

int main() {
    uint32_t number;
    std::cin >> number;
    if (!number || ((number - 1) & number)) {
        std::cout << "FAIL";
    } else {
        std::cout << "OK";
    }
}
