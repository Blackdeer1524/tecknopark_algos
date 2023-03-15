#ifndef HEAP_HPP
#define HEAP_HPP

#include <cstdint>
#include <cstdlib>
#include <optional>
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
    explicit Heap(auto(*cmp)(T left, T right)->int) {
        m_cmp      = cmp;
        m_length   = 0;
        m_capacity = 10;

        T *m_buffer =
            reinterpret_cast<T *>(operator new(sizeof(T) * m_capacity));
    }

    ~Heap() {
        delete[] m_buffer;  // вроде не должно работать из-за UB (вызов
                            // деструктора на сырых данных)
    }

    auto insert(T item) -> bool {
        if (m_length == m_capacity) {
            m_capacity <<= 1;
            T *new_buffer = reinterpret_cast<T *>(
                std::realloc(m_buffer, sizeof(T) * m_capacity));
            if (new_buffer == nullptr) {
                return true;
            }
            m_buffer = new_buffer;
        }
        m_buffer[m_length] = item;
        this->shift_up(m_length);
        ++m_length;
        return false;
    }

    auto pop_min() -> std::optional<T>;

    // auto get_min() const -> void;

 private:
    T       *m_buffer;
    uint64_t m_length;
    uint64_t m_capacity;
    auto(*m_cmp)(T left, T right) -> int;

    auto shift_up(uint64_t shifting_item_index) -> void {
        while (shifting_item_index) {
            auto parent = get_parent_index(shifting_item_index);
            if (m_cmp(shifting_item_index, parent) <= 0) {
                return;
            }
            std::swap(m_buffer[shifting_item_index], m_buffer[parent]);
            shifting_item_index = parent;
        }
    }

    auto heapify(uint64_t parent_index) -> void {
        while (true) {
            auto origin = parent_index;
            auto left   = get_left_child_index(parent_index);
            auto right  = get_right_child_index(parent_index);

            if (left < m_capacity &&
                m_cmp(m_buffer[parent_index], m_buffer[left]) > 0) {
                parent_index = left;
            }
            if (right < m_capacity &&
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
