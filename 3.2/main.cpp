#include <compare>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <tuple>

struct Deck {
    Deck() {
        capacity_   = 5;
        buffer_     = new int[capacity_];
        push_index_ = 0;
        pop_index_  = 0;
    }

    Deck(Deck &&other) {
        capacity_   = other.capacity_;
        buffer_     = other.buffer_;
        push_index_ = other.push_index_;
        pop_index_  = other.pop_index_;
    }

    auto operator=(Deck &&other) -> Deck & {
        delete[] buffer_;
        capacity_   = other.capacity_;
        buffer_     = other.buffer_;
        push_index_ = other.push_index_;
        pop_index_  = other.pop_index_;
        return *this;
    }

    ~Deck() {
        delete[] buffer_;
    }

    [[nodiscard]] auto push(int item) -> bool {
        buffer_[push_index_] = item;
        push_index_          = (push_index_ + 1) % capacity_;
        if (push_index_ == pop_index_) {
            auto n_items_before_pop_index_exclusive = pop_index_;
            auto n_items_after_pop_index_inclusive  = capacity_ - pop_index_;
            capacity_ <<= 1;

            auto new_buffer = new int[capacity_];
            std::memcpy(new_buffer,
                        buffer_ + n_items_before_pop_index_exclusive,
                        n_items_after_pop_index_inclusive * sizeof(int));
            std::memcpy(new_buffer + n_items_after_pop_index_inclusive,
                        buffer_,
                        n_items_before_pop_index_exclusive * sizeof(int));
            delete[] buffer_;
            buffer_     = new_buffer;
            push_index_ = 0;
            pop_index_  = 0;
        }
        return false;
    }

    [[nodiscard]] auto pop() -> std::tuple<int, bool> {
        if (pop_index_ == push_index_) {
            return std::make_tuple(0, true);
        }
        auto res   = buffer_[pop_index_++];
        pop_index_ = (pop_index_ + 1) % capacity_;
        return std::make_tuple(res, false);
    }

 private:
    int     *buffer_;
    uint64_t capacity_;
    uint64_t push_index_;
    uint64_t pop_index_;
};

auto main() -> int {
}
