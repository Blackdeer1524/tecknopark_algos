#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <ostream>
#include <string.h>
#include <string>

// IMAGINE писать не safe код

static constexpr int ALPHABET_SIZE =
    static_cast<int>(std::numeric_limits<unsigned char>::max()) + 1;

struct String {
    String() {
        buffer_ = nullptr;
        length_ = 0;
    }

    String(char *str, uint64_t length) {
        buffer_ = new char[length];
        strncpy(buffer_, str, length);
        length_ = length;
    }

    auto get_char(uint64_t index) const -> int {
        if (index >= length_) {
            return 0;
        }
        return buffer_[index] - std::numeric_limits<char>::min();
    }

    auto c_str() -> const char * {
        return reinterpret_cast<char *>(buffer_);
    }

 private:
    char    *buffer_;
    uint64_t length_;
};

static auto
msd_string_sort_internals(String **A,  // что сортируем
                          String **B,  // куда класть результат сортировки
                          uint64_t l,
                          uint64_t r,
                          uint64_t sorting_index = 0) -> void {
    if (l == r) {
        return;
    }
    auto A_buffer = *A;
    auto B_buffer = *B;

    uint64_t buffer[ALPHABET_SIZE] = {};
    for (uint64_t i = l; i <= r; ++i) {
        auto str = A_buffer[i];
        ++buffer[str.get_char(sorting_index)];
    }
    for (int i = 1; i < ALPHABET_SIZE; i++) {
        buffer[i] += buffer[i - 1];
    }

    for (uint64_t i = r; i > l; --i) {
        auto cur_char                = A_buffer[i].get_char(sorting_index);
        B_buffer[--buffer[cur_char]] = A_buffer[i];
    }
    auto cur_char                = A_buffer[l].get_char(sorting_index);
    B_buffer[--buffer[cur_char]] = A_buffer[l];

    auto placeholder = *A;
    *A               = *B;
    *B               = placeholder;

    // auto new_l                = l;
    // auto sorting_group_letter = A_buffer[l].get_char(sorting_index);
    // for (uint64_t i = l + 1; i <= r; ++i) {
    //     auto current_letter = A_buffer[i].get_char(sorting_index);
    //     if (current_letter != sorting_group_letter) {
    //         msd_string_sort_internals(A, B, new_l, i - 1, sorting_index + 1);
    //         new_l                = i;
    //         sorting_group_letter = current_letter;
    //     }
    // }
    // if (sorting_group_letter == '\0') {
    //     return;
    // }
    //
    // msd_string_sort_internals(A, B, new_l, r, sorting_index + 1);
}

auto mds_string_sort(String *A, uint64_t length) -> void {
    if (length == 0) {
        return;
    }
    auto *B = new String[length];

    msd_string_sort_internals(&A, &B, 0, length - 1);

    delete[] B;
}

static constexpr uint64_t STRING_READING_BUFFER_LENGTH = 0x1000;

auto main() -> int {
    char    *line = nullptr;
    uint64_t len  = 0;

    uint64_t cont_capacity = 1;
    String  *container =
        reinterpret_cast<String *>(std::malloc(sizeof(String) * cont_capacity));
    uint64_t cont_len = 0;
    for (int i = 0; i < 4; ++i) {
        // while (true) {
        ssize_t nread = getline(&line, &len, stdin);
        if (cont_len == cont_capacity) {
            cont_capacity <<= 1;
            String *new_container = reinterpret_cast<String *>(
                std::realloc(container, sizeof(String) * cont_capacity));
            if (new_container == NULL) {
                std::free(line);
                std::free(container);
                return EXIT_FAILURE;
            }
            container = new_container;
        }
        line[nread - 1] = '\0';

        if (line[0] == '\0') {
            break;
        }
        container[cont_len++] = String(line, nread);
    }
    mds_string_sort(container, cont_len);
    for (uint64_t i = 0; i < cont_len; ++i) {
        std::cout << container[i].c_str() << std::endl;
    }

    free(line);
    std::free(container);
}
