#include <compare>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <tuple>

struct Deck {
    Deck() {
        capacity_    = 2;
        length_      = 0;
        buffer_      = new int[capacity_];
        back_index_  = capacity_ - 1;
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

    auto push_front(int item) -> void {
        buffer_[front_index_] = item;
        front_index_          = (front_index_ + 1) % capacity_;
        ++length_;
        if (length_ == capacity_) {
            this->resize();
        }
    }

    auto push_back(int item) -> void {
        buffer_[back_index_] = item;
        back_index_ = (back_index_ == 0) ? capacity_ - 1 : back_index_ - 1;
        ++length_;
        if (length_ == capacity_) {
            this->resize();
        }
    }

    [[nodiscard]] auto pop_front() -> std::optional<int> {
        if (!length_) {
            return std::nullopt;
        }
        --length_;
        front_index_ = (front_index_ == 0) ? capacity_ - 1 : front_index_ - 1;
        return buffer_[front_index_];
    }

    [[nodiscard]] auto pop_back() -> std::optional<int> {
        if (!length_) {
            return std::nullopt;
        }
        --length_;
        back_index_ = (back_index_ + 1) % capacity_;
        return buffer_[back_index_];
    }

 private:
    int     *buffer_;
    uint64_t capacity_;
    uint64_t length_;
    uint64_t back_index_;
    uint64_t front_index_;

    auto resize() -> void {
        auto new_buffer = new int[capacity_ << 1];
        std::memcpy(new_buffer, buffer_, sizeof(int) * front_index_);

        auto n_back_items = capacity_ - front_index_;
        capacity_ <<= 1;
        back_index_ = capacity_ - n_back_items - 1;

        std::memcpy(new_buffer + back_index_ + 1,
                    buffer_ + front_index_,
                    sizeof(int) * n_back_items);
        delete[] buffer_;
        buffer_ = new_buffer;
    }
};

enum Operation {
    PushFront = 1,
    PopFront,
    PushBack,
    PopBack,
};

auto main() -> int {
    int N, operation, operand;
    std::cin >> N;
    auto deck = Deck();
    for (uint64_t i = 0; i < N; ++i) {
        std::cin >> operation >> operand;
        switch (operation) {
            case PushFront: {
                deck.push_front(operand);
                break;
            }
            case PushBack: {
                deck.push_back(operand);
                break;
            }
            case PopFront: {
                auto res = deck.pop_front();
                if (!(res.has_value() && res.value() == operand ||
                      operand == -1 && !res.has_value())) {
                    std::cout << "NO";
                    exit(0);
                }
                break;
            }
            case PopBack: {
                auto res = deck.pop_back();
                if (!(res.has_value() && res.value() == operand ||
                      operand == -1 && !res.has_value())) {
                    std::cout << "NO";
                    exit(0);
                }
                break;
            }
            default: {
                std::cout << "UNKNOWN OPERATION: (" << operation << ", "
                          << operand << ")";
                exit(1);
            }
        }
    }
    std::cout << "YES";
}
