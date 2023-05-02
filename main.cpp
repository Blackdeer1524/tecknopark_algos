#include <iostream>

int main() {
    uint32_t number, query;
    std::cin >> number >> query;
    auto result = (number >> query) & 1;
    std::cout << result;
}
