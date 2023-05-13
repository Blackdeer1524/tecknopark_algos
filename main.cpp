#include <algorithm>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <utility>

// Вариант 1: Обход дерева в порядке in-order

struct Node {
    explicit Node(int value) : value_(value), left_(nullptr), right_(nullptr) {
    }

    int   value_;
    Node *left_;
    Node *right_;
};

class BinaryTree {
 public:
    explicit BinaryTree(std::function<int(int left, int right)> cmp)
        : cmp_(std::move(cmp)) {
    }

    ~BinaryTree() {
        if (root_ == nullptr) {
            return;
        }
        auto deletion_stack = std::stack<Node *>();
        deletion_stack.push(root_);
        while (!deletion_stack.empty()) {
            auto *node = deletion_stack.top();
            deletion_stack.pop();
            if (node->left_ != nullptr) {
                deletion_stack.push(node->left_);
            }
            if (node->right_ != nullptr) {
                deletion_stack.push(node->right_);
            }
            delete node;
        }
    }

    auto insert(int value) -> void {
        if (root_ == nullptr) {
            auto *node = new Node(value);
            root_      = node;
            return;
        }
        auto *parent  = root_;
        auto *current = root_;
        while (current != nullptr) {
            parent = current;
            if (cmp_(value, current->value_) >= 0) {
                current = current->right_;
            } else {
                current = current->left_;
            }
        }
        auto *new_node = new Node(value);
        if (cmp_(value, parent->value_) >= 0) {
            parent->right_ = new_node;
        } else {
            parent->left_ = new_node;
        }
    }

    auto print() -> void {
        if (root_ == nullptr) {
            return;
        }

        auto  printing_stack = std::stack<Node *>();
        auto *current        = root_;
        for (;;) {
            while (current != nullptr) {
                printing_stack.push(current);
                current = current->left_;
            }
            if (printing_stack.empty()) {
                break;
            }
            current = printing_stack.top();
            printing_stack.pop();
            std::cout << current->value_ << ' ';
            current = current->right_;
        }
    }

 private:
    Node                        *root_ = nullptr;
    std::function<int(int, int)> cmp_;
};

auto main() -> int {
    auto     tree = BinaryTree([](int left, int right) -> int {
        if (left > right) {
            return 1;
        }
        if (left < right) {
            return -1;
        }
        return 0;
    });
    uint64_t n;
    std::cin >> n;
    for (uint64_t i = 0; i < n; ++i) {
        int value;
        std::cin >> value;
        tree.insert(value);
    }
    tree.print();
}
