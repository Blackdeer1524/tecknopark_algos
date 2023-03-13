#include <cstdint>
#include <iostream>

int main() {
    uint32_t number, index;
    std::cin >> number >> index;
    auto result = number ^ (1 << index);
    std::cout << result;
}
