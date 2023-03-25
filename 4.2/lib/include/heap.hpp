#ifndef HEAP_HPP
#define HEAP_HPP

#include <cstdint>
#include <cstdlib>
#include <limits>
#include <optional>
#include <sys/types.h>
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
        // будем иметь место под K лучших элементов + их детей - 1
        m_buffer = reinterpret_cast<T *>(operator new(sizeof(T) * m_capacity));
    }

    ~Heap() {
        std::free(m_buffer);
    }

    auto insert(T &&item) -> bool {
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

    auto pop_min() -> std::optional<T> {
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
