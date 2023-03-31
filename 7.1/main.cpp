#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string.h>
#include <utility>

static constexpr int ALPHABET_SIZE =
    static_cast<int>(std::numeric_limits<unsigned char>::max()) + 1;

struct String {
    String() {
        buffer_ = nullptr;
        length_ = 0;
    }

    String(const char *str, uint64_t length) {
        buffer_ = new char[length + 1];
        strncpy(buffer_, str, length + 1);
        length_ = length;
    }

    String(const String &other) = delete;

    ~String() {
        delete[] buffer_;
    }

    auto operator=(String &&other) -> String & {
        delete[] buffer_;
        buffer_       = other.buffer_;
        length_       = other.length_;
        other.buffer_ = nullptr;
        other.length_ = 0;
        return *this;
    }

    auto get_char(uint64_t index) const -> int {
        if (index >= length_) {
            return 0;
        }
        return buffer_[index] - std::numeric_limits<char>::min();
    }

    auto c_str() const -> const char * {
        return buffer_;
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
        auto &str = A_buffer[i];
        ++buffer[str.get_char(sorting_index)];
    }
    for (int i = 1; i < ALPHABET_SIZE; i++) {
        buffer[i] += buffer[i - 1];
    }

    for (uint64_t i = r; i > l; --i) {
        auto cur_char                = A_buffer[i].get_char(sorting_index);
        B_buffer[--buffer[cur_char]] = std::move(A_buffer[i]);
    }
    auto cur_char                = A_buffer[l].get_char(sorting_index);
    B_buffer[--buffer[cur_char]] = std::move(A_buffer[l]);

    for (uint64_t i = l; i <= r; ++i) {
        A_buffer[i] = std::move(B_buffer[i - l]);
    }

    auto new_l                = l;
    auto sorting_group_letter = A_buffer[l].get_char(sorting_index);
    for (uint64_t i = l + 1; i <= r; ++i) {
        auto current_letter = A_buffer[i].get_char(sorting_index);
        if (current_letter != sorting_group_letter) {
            msd_string_sort_internals(A, B, new_l, i - 1, sorting_index + 1);
            new_l                = i;
            sorting_group_letter = current_letter;
        }
    }
    if (sorting_group_letter == 0) {
        return;
    }

    msd_string_sort_internals(A, B, new_l, r, sorting_index + 1);
}

auto mds_string_sort(String *A, uint64_t length) -> void {
    if (length == 0) {
        return;
    }
    auto *B = new String[length];

    msd_string_sort_internals(&A, &B, 0, length - 1);

    delete[] B;
}

auto main() -> int {
    char    *line = nullptr;
    uint64_t len  = 0;

    uint64_t cont_capacity = 1;
    auto     container     = new String[cont_capacity];
    uint64_t cont_len      = 0;
    while (true) {
        ssize_t nread = getline(&line, &len, stdin);
        if (nread == -1) {
            break;
        }

        if (cont_len == cont_capacity) {
            String *new_container = new String[cont_capacity << 1];
            for (uint64_t i = 0; i < cont_capacity; i++) {
                new_container[i] = std::move(container[i]);
            }
            delete[] container;
            container = new_container;
            cont_capacity <<= 1;
        }

        line[--nread]         = '\0';
        container[cont_len++] = String(line, nread);
    }
    mds_string_sort(container, cont_len);
    for (uint64_t i = 0; i < cont_len; ++i) {
        std::cout << container[i].c_str() << std::endl;
    }

    free(line);
    delete[] container;
}
