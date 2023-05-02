#include <cstdint>
#include <iostream>

int main() {
    uint32_t input = 0;
    std::cin >> input;
    int count = 0;
    for (int i = 0; i < 32; i += 2) {
        count += input & 1;
        input >>= 2;
    }
    std::cout << count;
}
