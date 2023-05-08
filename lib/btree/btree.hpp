#ifndef BTREE_H
#define BTREE_H

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <utility>
#include <vector>

// using T = uint32_t;
template <typename T>
class BTree {
 public:
    static auto build(uint64_t t, std::function<int(const T &, const T &)> cmp)
        -> std::unique_ptr<BTree> {
        auto tree = std::unique_ptr<BTree>(nullptr);
        if (t <= 1) {
            return tree;
        }
        tree.reset(new BTree(t, std::move(cmp)));
        return tree;
    }

    auto insert(T &&item) -> void {
        if (root_->full()) {
            split_root();
        }
        root_->insert_nonfull(std::move(item));
    }

    auto print() const -> void {
        if (root_ == nullptr) {
            return;
        }
        auto     print_queue = std::queue<std::pair<Node *, uint64_t>>();
        uint64_t cur_level   = 0;
        print_queue.emplace(root_.get(), cur_level);
        while (!print_queue.empty()) {
            auto [item, level] = print_queue.front();
            print_queue.pop();
            if (level > cur_level) {
                cur_level = level;
                std::cout << std::endl;
            }
            for (uint64_t i = 0; i < item->size(); ++i) {
                std::cout << item->keys_.at(i) << ' ';
            }

            if (!item->leaf_) {
                for (uint64_t i = 0; i <= item->size(); ++i) {
                    print_queue.emplace(item->children_.at(i).get(), level + 1);
                }
            }
        }
    }

 private:
    BTree(uint64_t t, std::function<int(const T &, const T &)> cmp)
        : t_(t), cmp_(std::move(cmp)),
          root_(std::make_unique<Node>(0, t, true, std::move(cmp))) {
    }

    class Node {
     public:
        explicit Node(uint64_t                                        n,
                      uint64_t                                        t,
                      bool                                            leaf,
                      const std::function<int(const T &, const T &)> &cmp)
            : leaf_(leaf), t_(t), keys_(n), children_(n + 1), cmp_(cmp) {
        }

        [[nodiscard]] auto search(const T &k) const -> bool {
            uint64_t i = 0;
            while (i < size() && cmp_(k, keys_.at(i)) > 0) {
                ++i;
            }
            if (i < size() && cmp_(k, keys_.at(i)) == 0) {
                return true;
            }
            if (leaf_) {
                return false;
            }
            return children_.at(i)->search(k);
        }

        auto split_full_child(uint64_t child_index) -> void {
            Node                 *child = children_.at(child_index).get();
            std::unique_ptr<Node> new_node =
                std::make_unique<Node>(t_ - 1, t_, child->leaf_, cmp_);

            for (uint64_t i = 0; i < t_ - 1; ++i) {
                new_node->keys_.at(i) = std::move(child->keys_.at(i + t_));
            }

            if (!child->leaf_) {
                for (uint64_t i = 0; i < t_; ++i) {
                    new_node->children_.at(i) =
                        std::move(child->children_.at(i + t_));
                }
                child->children_.resize(t_);
            }
            auto median = std::move(child->keys_.at(t_ - 1));
            child->keys_.resize(t_ - 1);

            keys_.resize(size() + 1);
            if (size() >= 2) {
                for (uint64_t i = size() - 2; i > child_index; --i) {
                    keys_.at(i + 1) = std::move(keys_.at(i));
                }
            }
            if (child_index + 1 < size()) {
                keys_.at(child_index + 1) = keys_.at(child_index);
            }
            keys_.at(child_index) = median;

            children_.resize(size() + 1);
            for (uint64_t i = size() - 1; i > child_index + 1; --i) {
                children_.at(i) = std::move(children_.at(i - 1));
            }
            children_.at(child_index + 1) = std::move(new_node);
        }

        auto insert_nonfull(T &&k) -> void {
            uint64_t i = size() - 1;
            if (leaf_) {
                keys_.resize(size() + 1);
                children_.resize(size() + 1);
                if (size() > 1) {
                    while (i > 0 && cmp_(k, keys_.at(i)) < 0) {
                        keys_.at(i + 1) = keys_.at(i);
                        --i;
                    }
                    if (i == 0 && cmp_(k, keys_.at(0)) < 0) {
                        keys_.at(i + 1) = keys_.at(i);
                        --i;
                    }
                }
                keys_.at(i + 1) = k;
            } else {
                while (i > 0 && cmp_(k, keys_.at(i)) < 0) {
                    --i;
                }
                if (i == 0 && cmp_(k, keys_.at(0)) < 0) {
                    --i;
                }
                ++i;
                auto *child = children_.at(i).get();
                if (child->full()) {
                    split_full_child(i);
                    if (cmp_(k, keys_.at(i)) > 0) {
                        ++i;
                        child = children_.at(i).get();
                    }
                }
                child->insert_nonfull(std::move(k));
            }
        }

        [[nodiscard]] auto size() const -> uint64_t {
            return keys_.size();
        }

        [[nodiscard]] auto full() const -> bool {
            return size() == (t_ << 1) - 1;
        }

        // private:
        uint64_t                                        t_;
        bool                                            leaf_;
        std::vector<T>                                  keys_;
        std::vector<std::unique_ptr<Node>>              children_;
        const std::function<int(const T &, const T &)> &cmp_;
    };

    auto split_root() -> void {
        auto new_root             = std::make_unique<Node>(0, t_, false, cmp_);
        new_root->children_.at(0) = std::move(root_);
        root_                     = std::move(new_root);
        root_->split_full_child(0);
    }

    std::unique_ptr<Node>                    root_;
    uint64_t                                 t_;
    std::function<int(const T &, const T &)> cmp_;
};

#endif  // !BTREE_H
