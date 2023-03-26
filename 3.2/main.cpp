#include <compare>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <tuple>

struct Deck {
    Deck() {
        capacity_    = 5;
        buffer_      = new int[capacity_];
        back_index_  = 0;
        front_index_ = 0;
    }

    ~Deck() {
        delete[] buffer_;
    }

    Deck(const Deck &other) = delete;

    Deck(Deck &&other) {
        capacity_    = other.capacity_;
        buffer_      = other.buffer_;
        back_index_  = other.back_index_;
        front_index_ = other.front_index_;
    }

    auto operator=(Deck &&other) -> Deck & {
        delete[] buffer_;
        capacity_    = other.capacity_;
        buffer_      = other.buffer_;
        back_index_  = other.back_index_;
        front_index_ = other.front_index_;
        return *this;
    }

    auto push_back(int item) -> void {
        buffer_[back_index_] = item;
        back_index_          = (back_index_ + 1) % capacity_;
        if (back_index_ == front_index_) {
            this->resize();
        }
    }

    auto push_front(int item) -> void {
        front_index_ = (front_index_ == 0) ? capacity_ - 1 : front_index_ - 1;
        if (front_index_ == back_index_) {
            this->resize();
        }

        buffer_[front_index_] = item;
    }

    [[nodiscard]] auto pop_front() -> std::tuple<int, bool> {
        if (front_index_ == back_index_) {
            return std::make_tuple(0, true);
        }
        auto res     = buffer_[front_index_];
        front_index_ = (front_index_ + 1) % capacity_;
        return std::make_tuple(res, false);
    }

    [[nodiscard]] auto pop_back() -> std::tuple<int, bool> {
        if (front_index_ == back_index_) {
            return std::make_tuple(0, true);
        }
        auto res    = buffer_[back_index_];
        back_index_ = (back_index_ == 0) ? capacity_ - 1 : back_index_ - 1;
        return std::make_tuple(res, false);
    }

 private:
    int     *buffer_;
    uint64_t capacity_;
    uint64_t back_index_;
    uint64_t front_index_;

    auto resize() -> void {
        auto n_items_before_front_index_exclusive = front_index_;
        auto n_items_after_front_index_inclusive  = capacity_ - front_index_;
        capacity_ <<= 1;

        auto new_buffer = new int[capacity_];
        std::memcpy(new_buffer,
                    buffer_ + n_items_before_front_index_exclusive,
                    n_items_after_front_index_inclusive * sizeof(int));
        std::memcpy(new_buffer + n_items_after_front_index_inclusive,
                    buffer_,
                    n_items_before_front_index_exclusive * sizeof(int));
        delete[] buffer_;
        buffer_      = new_buffer;
        back_index_  = 0;
        front_index_ = 0;
    }
};

auto main() -> int {
}
