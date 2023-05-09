#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <ios>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

using T = std::string;

class AVLTree {
    class Node {
     public:
        enum class BALANCING_FACTOR {
            NEGATIVE_TWO,
            NEGATIVE_ONE,
            NEUTRAL,
            POSITIVE_ONE,
            POSITIVE_TWO,
        };

        T                     value_;
        int                   height_{};
        uint64_t              size_{};
        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;

        explicit Node(T &&value)
            : value_(std::move(value)), left_(nullptr), right_(nullptr) {
        }

        [[nodiscard]] auto is_leaf() const -> bool {
            return left_ == nullptr && right_ == nullptr;
        }

        auto fix_height() -> void {
            height_ = std::max(left_->height_, right_->height_) + 1;
        }

        [[nodiscard]] auto bfactor() const -> BALANCING_FACTOR {
            if (left_->size_ > right_->size_) {
                switch (left_->size_ - right_->size_) {
                    case 1:
                        return BALANCING_FACTOR::POSITIVE_ONE;
                    case 2:
                        return BALANCING_FACTOR::POSITIVE_TWO;
                    default:
                        throw std::runtime_error("unreachable");
                        break;
                }
            } else {
                switch (right_->size_ - left_->size_) {
                    case 1:
                        return BALANCING_FACTOR::NEGATIVE_ONE;
                    case 2:
                        return BALANCING_FACTOR::NEGATIVE_TWO;
                    default:
                        throw std::runtime_error("unreachable");
                        break;
                }
            }
        }
    };

 public:
    using comparison_fn_t = std::function<int(const T &, const T &)>;

    explicit AVLTree(comparison_fn_t cmp) : cmp_(std::move(cmp)) {
    }

    auto insert(T &&item) -> void {
        root_ = insert_aux(std::move(root_), std::move(item));
    }

    auto remove(const T &item) -> void {
        root_ = remove_aux(std::move(root_), item);
    }

    auto find(uint64_t statistic) -> const T &;

 private:
    static auto balance(std::unique_ptr<Node> &&balancing_node)
        -> std::unique_ptr<Node> {
        balancing_node->fix_height();

        auto balancing_factor = balancing_node->bfactor();
        if (balancing_factor == Node::BALANCING_FACTOR::POSITIVE_TWO) {
            if (balancing_node->left_->right_->height_ >
                balancing_node->left_->left_->height_) {
                balancing_node->left_ =
                    rotate_left(std::move(balancing_node->left_));
            }
            balancing_node = rotate_right(std::move(balancing_node));
        }
        if (balancing_factor == Node::BALANCING_FACTOR::NEGATIVE_TWO) {
            if (balancing_node->right_->left_->height_ >
                balancing_node->right_->right_->height_) {
                balancing_node->right_ =
                    rotate_right(std::move(balancing_node->right_));
            }
            balancing_node = rotate_left(std::move(balancing_node));
        }
        return balancing_node;
    }

    static auto rotate_left(std::unique_ptr<Node> &&axis)
        -> std::unique_ptr<Node> {
        auto tmp     = std::move(axis->right_);
        axis->right_ = std::move(axis->right_->left_);
        tmp->left_   = std::move(axis);
        return tmp;
    }

    static auto rotate_right(std::unique_ptr<Node> &&axis)
        -> std::unique_ptr<Node> {
        auto tmp    = std::move(axis->left_);
        axis->left_ = std::move(axis->left_->right_);
        tmp->right_ = std::move(axis);
        return tmp;
    }

    auto insert_aux(std::unique_ptr<Node> dst, T &&item)
        -> std::unique_ptr<Node> {
        if (dst == nullptr) {
            return std::make_unique<Node>(std::move(item));
        }
        auto cmp_res = cmp_(dst->value_, item);
        if (cmp_res > 0) {
            dst->left_ = insert_aux(std::move(dst->left_), std::move(item));
        } else if (cmp_res < 0) {
            dst->right_ = insert_aux(std::move(dst->right_), std::move(item));
        }
        return balance(std::move(dst));
    }

    auto remove_aux(std::unique_ptr<Node> &&dst, const T &item)
        -> std::unique_ptr<Node> {
        if (dst == nullptr) {
            return nullptr;
        }

        auto cmp_res = cmp_(dst->value_, item);
        if (cmp_res < 0) {
            dst->right_ = remove_aux(std::move(dst->right_), item);
        } else if (cmp_res > 0) {
            dst->left_ = remove_aux(std::move(dst->left_), item);
        } else {
            if (dst->is_leaf()) {
                return nullptr;
            }
            if (dst->right_ == nullptr) {
                dst = std::move(dst->left_);
                return dst;
            }
            auto min_value = find_min(dst->right_.get());
            dst->right_    = remove_min_node(std::move(dst->right_));
            dst->value_    = min_value;
        }
        return balance(std::move(dst));
    }

    static auto find_min(Node *src) -> T {
        while (src->left_ != nullptr) {
            src = src->left_.get();
        }
        return src->value_;
    }

    auto remove_min_node(std::unique_ptr<Node> &&src) -> std::unique_ptr<Node> {
        if (src->left_ == nullptr) {
            return std::move(src->right_);
        }
        src->left_ = remove_min_node(std::move(src->left_));
        return balance(std::move(src));
    }

    comparison_fn_t       cmp_;
    std::unique_ptr<Node> root_;
};

#endif  // !AVL_TREE_H
