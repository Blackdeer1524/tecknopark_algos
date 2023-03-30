#include <cstdint>
#include <cstdio>
#include <limits>
#include <string.h>
#include <type_traits>

static constexpr int ALPHABET_SIZE =
    static_cast<int>(std::numeric_limits<unsigned char>::max()) + 1;

struct String {
    unsigned char *buffer;
    uint64_t       length;

    auto get_char(uint64_t index) const -> int {
        if (index >= length) {
            return static_cast<int>(std::numeric_limits<unsigned char>::max()) +
                   1;
        }
        return buffer[index];
    }
};

static auto
msd_string_sort_internals(String *&A,  // что сортируем
                          String *&B,  // куда класть результат сортировки
                          uint64_t l,
                          uint64_t r,
                          uint64_t sorting_index = 0) -> void {
    if (l == r) {
        return;
    }

    uint64_t buffer[ALPHABET_SIZE] = {};
    for (uint64_t i = l; i <= r; ++i) {
        auto &str = A[i];
        ++buffer[str.get_char(sorting_index)];
    }
    for (int i = 1; i < ALPHABET_SIZE; i++) {
        buffer[i] += buffer[i - 1];
    }

    for (uint64_t i = r; i > l; --i) {
        auto sorting_char          = A[i].get_char(sorting_index);
        B[--buffer[sorting_index]] = A[i];
    }
    auto sorting_char          = A[l].get_char(sorting_index);
    B[--buffer[sorting_index]] = A[l];

    auto placeholder = A;
    A                = B;
    B                = A;

    auto new_l                = l;
    auto sorting_group_letter = A[l].get_char(sorting_index);
    for (uint64_t i = l + 1; i <= r; ++i) {
        auto current_letter = A[i].get_char(sorting_index);
        if (current_letter != sorting_group_letter) {
            msd_string_sort_internals(A, B, new_l, i - 1);
            new_l                = i;
            sorting_group_letter = current_letter;
        }
    }
    if (sorting_group_letter == '\0') {
        return;
    }

    msd_string_sort_internals(A, B, new_l, r);
}

auto mds_string_sort(String *&A, uint64_t length) -> void {
    if (length == 0) {
        return;
    }
    auto *B = new String[length];

    msd_string_sort_internals(A, B, 0, length - 1);

    delete[] B;
}

auto main() -> int {
}
