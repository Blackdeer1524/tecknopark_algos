#ifndef HEAP_HPP
#define HEAP_HPP

#include <cstdint>
#include <cstdlib>
#include <limits>
#include <optional>
#include <sys/types.h>
#include <tuple>
#include <utility>

static inline auto get_parent_index(uint64_t child_index) -> uint64_t {
    return (child_index - 1) >> 1;
}

static inline auto get_left_child_index(uint64_t parent_index) -> uint64_t {
    return (parent_index << 1) + 1;
}

static inline auto get_right_child_index(uint64_t parent_index) -> uint64_t {
    return (parent_index + 1) << 1;
}

template <typename T>
struct Heap {
    explicit Heap(auto(*cmp)(const T &left, const T &right)->int) {
        m_cmp      = cmp;
        m_length   = 0;
        m_capacity = 1;
        m_buffer = reinterpret_cast<T *>(operator new(sizeof(T) * m_capacity));
    }

    ~Heap() {
        std::free(m_buffer);
    }

    auto push(T &&item) -> bool {
        if (m_length == m_capacity) {
            m_capacity <<= 1;
            T *new_buffer = reinterpret_cast<T *>(
                std::realloc(m_buffer, sizeof(T) * m_capacity));
            if (new_buffer == nullptr) {
                return true;
            }
            m_buffer = new_buffer;
        }
        m_buffer[m_length] = std::move(item);
        this->shift_up(m_length);
        ++m_length;
        return false;
    }

    // В хипе пайтона есть такой метод. Чем я хуже?)
    auto push_pop(T &&item) -> T {
        auto popped_item = std::move(m_buffer[0]);
        m_buffer[0]      = std::move(item);
        this->heapify(0);
        return popped_item;
    }

    auto pop() -> std::optional<T> {
        if (!m_length) {
            return std::nullopt;
        }
        auto res    = std::move(m_buffer[0]);
        m_buffer[0] = m_buffer[--m_length];
        this->heapify(0);
        return res;
    }

    auto get_min() -> std::optional<T> {
        if (m_length) {
            return m_buffer[0];
        }
        return std::nullopt;
    }

 private:
    T       *m_buffer;
    uint64_t m_length;
    uint64_t m_capacity;

    auto(*m_cmp)(const T &left, const T &right) -> int;

    auto shift_up(uint64_t shifting_item_index) -> void {
        while (shifting_item_index) {
            auto parent = get_parent_index(shifting_item_index);
            if (m_cmp(m_buffer[parent], m_buffer[shifting_item_index]) <= 0) {
                return;
            }
            std::swap(m_buffer[parent], m_buffer[shifting_item_index]);
            shifting_item_index = parent;
        }
    }

    auto heapify(uint64_t parent_index) -> void {
        while (true) {
            auto origin = parent_index;
            auto left   = get_left_child_index(parent_index);
            auto right  = get_right_child_index(parent_index);

            if (left < m_length &&
                m_cmp(m_buffer[parent_index], m_buffer[left]) > 0) {
                parent_index = left;
            }
            if (right < m_length &&
                m_cmp(m_buffer[parent_index], m_buffer[right]) > 0) {
                parent_index = right;
            }

            if (parent_index == origin) {
                break;
            }
            std::swap(m_buffer[origin], m_buffer[parent_index]);
        }
    }
};

#endif

#include <iostream>

using UserData = std::pair<uint64_t, uint64_t>;

int main() {
    auto min_heap_cmp = [](const UserData &left, const UserData &right) -> int {
        if (left.second == right.second) {
            return 0;
        } else if (left.second < right.second) {
            return -1;
        } else {
            return 1;
        }
    };
    uint64_t N, K;
    std::cin >> N >> K;
    if (N < K) {
        return -1;
    }
    auto maxheap = Heap<UserData>(min_heap_cmp);
    for (uint64_t i = 0; i < K; ++i) {
        UserData user_data;
        std::cin >> user_data.first >> user_data.second;
        maxheap.push(std::move(user_data));
    }

    auto heap_min = maxheap.get_min().value().second;
    for (uint64_t i = K; i < N; ++i) {
        UserData user_data;
        std::cin >> user_data.first >> user_data.second;
        if (heap_min < user_data.second) {
            maxheap.push_pop(std::move(user_data));
            heap_min = maxheap.get_min().value().second;
        }
    }

    for (uint64_t i = 0; i < K; ++i) {
        auto res_opt = maxheap.pop();
        std::cout << res_opt->first << ' ';
    }
}
