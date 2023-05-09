#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <algorithm>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <ios>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

using T = int;

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
        int                   height_{1};
        uint64_t              size_{1};
        std::unique_ptr<Node> left_;
        std::unique_ptr<Node> right_;

        explicit Node(T &&value)
            : value_(std::move(value)), left_(nullptr), right_(nullptr) {
        }

        [[nodiscard]] auto is_leaf() const -> bool {
            return left_ == nullptr && right_ == nullptr;
        }

        auto fix() -> void {
            fix_height();
            fix_size();
        }

        auto fix_height() -> void {
            auto left_height  = left_ == nullptr ? 0 : left_->height_;
            auto right_height = right_ == nullptr ? 0 : right_->height_;
            height_           = std::max(left_height, right_height) + 1;
        }

        auto fix_size() -> void {
            auto left_size  = left_ == nullptr ? 0 : left_->size_;
            auto right_size = right_ == nullptr ? 0 : right_->size_;
            size_           = left_size + right_size + 1;
        }

        [[nodiscard]] auto bfactor() const -> BALANCING_FACTOR {
            auto left_height  = left_ == nullptr ? 0 : left_->height_;
            auto right_height = right_ == nullptr ? 0 : right_->height_;
            if (left_height > right_height) {
                switch (left_height - right_height) {
                    case 0:
                        return BALANCING_FACTOR::NEUTRAL;
                    case 1:
                        return BALANCING_FACTOR::POSITIVE_ONE;
                    case 2:
                        return BALANCING_FACTOR::POSITIVE_TWO;
                    default:
                        break;
                }
            } else {
                switch (right_height - left_height) {
                    case 0:
                        return BALANCING_FACTOR::NEUTRAL;
                    case 1:
                        return BALANCING_FACTOR::NEGATIVE_ONE;
                    case 2:
                        return BALANCING_FACTOR::NEGATIVE_TWO;
                    default:
                        break;
                }
            }
            throw std::runtime_error("unreachable");
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

    auto find(Node *src, uint64_t statistic) -> std::optional<T> {
        if (src == nullptr) {
            return std::nullopt;
        }
        auto node_order = (src->left_ == nullptr) ? 0 : src->left_->size_;
        if (statistic == node_order) {
            return src->value_;
        }
        if (statistic < node_order) {
            return find(src->left_.get(), statistic);
        }
        return find(src->right_.get(), statistic - node_order - 1);
    }

    auto in_order_print() {
        print(root_.get(), 0);
    }

 private:
    auto print(Node *node, int depth) -> void {
        if (node == nullptr) {
            return;
        }
        print(node->right_.get(), depth + 1);
        for (int i = 0; i < depth; ++i) {
            std::cout << "\t  ";
        }
        std::cout << '(' << node->value_ << "," << node->height_ << ","
                  << node->size_ << ')';
        std::cout << std::endl;
        print(node->left_.get(), depth + 1);
    }

    static auto balance(std::unique_ptr<Node> &&balancing_node)
        -> std::unique_ptr<Node> {
        balancing_node->fix();

        auto balancing_factor = balancing_node->bfactor();
        if (balancing_factor == Node::BALANCING_FACTOR::POSITIVE_TWO) {
            if (balancing_node->left_->bfactor() ==
                Node::BALANCING_FACTOR::NEGATIVE_ONE) {
                balancing_node->left_ =
                    rotate_left(std::move(balancing_node->left_));
            }
            balancing_node = rotate_right(std::move(balancing_node));
        } else if (balancing_factor == Node::BALANCING_FACTOR::NEGATIVE_TWO) {
            if (balancing_node->right_->bfactor() ==
                Node::BALANCING_FACTOR::POSITIVE_ONE) {
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
        axis->right_ = std::move(tmp->left_);
        tmp->left_   = std::move(axis);

        tmp->left_->fix();
        tmp->fix();
        return tmp;
    }

    static auto rotate_right(std::unique_ptr<Node> &&axis)
        -> std::unique_ptr<Node> {
        auto tmp    = std::move(axis->left_);
        axis->left_ = std::move(tmp->right_);
        tmp->right_ = std::move(axis);

        tmp->right_->fix();
        tmp->fix();
        return tmp;
    }

    auto insert_aux(std::unique_ptr<Node> &&dst, T &&item)
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
                return dst;  // Можно вернуть т.к. у левого потомка ничего не
                             // поменялось
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
