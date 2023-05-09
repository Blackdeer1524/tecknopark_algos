#include <gtest/gtest.h>

#include "avltree.hpp"
#include <utility>

template <typename T>
auto basic_cmp(const T &left, const T &right) -> int {

    if (left < right) {
        return -1;
    }
    if (left > right) {
        return 1;
    }
    return 0;
}

using T = int;

class AVLTreeTest : public AVLTree {
 public:
    explicit AVLTreeTest(AVLTree::comparison_fn_t cmp)
        : AVLTree(std::move(cmp)) {
    }

    auto assert_height_property() -> void {
        assert_height_property_aux(root_.get());
    }

    auto assert_tree_property() -> void {
        assert_tree_property_aux(root_.get());
    }

 private:
    auto assert_height_property_aux(Node *node) -> void {
        if (node == nullptr) {
            return;
        }
        auto left_height  = node->left_ == nullptr ? 0 : node->left_->height_;
        auto right_height = node->right_ == nullptr ? 0 : node->right_->height_;
        ASSERT_LE(left_height - right_height, 2);
        ASSERT_GE(left_height - right_height, -2);
        assert_height_property_aux(node->left_.get());
        assert_height_property_aux(node->right_.get());
    }

    auto assert_tree_property_aux(Node *node) -> void {
        if (node == nullptr) {
            return;
        }
        if (node->left_ != nullptr) {
            ASSERT_LE(node->left_->value_, node->value_);
            assert_height_property_aux(node->left_.get());
        }
        if (node->right_ != nullptr) {
            ASSERT_GE(node->right_->value_, node->value_);
            assert_height_property_aux(node->right_.get());
        }
    }
};

// INSERTION
TEST(ALVinsertions, NoExceptionsDuringSmallInsertionInOrder) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 0; i < 10; ++i) {
        EXPECT_NO_THROW(test.insert(std::move(i)));
    }
}

TEST(ALVinsertions, NoExceptionsDuringSmallInsertionReverseOrder) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 10; i > 0; --i) {
        EXPECT_NO_THROW(test.insert(std::move(i)));
    }
}

TEST(ALVinsertions, NoExceptionsDuringBigInsertionInOrder) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 0; i < 100; ++i) {
        EXPECT_NO_THROW(test.insert(std::move(i)));
    }
}

TEST(ALVinsertions, NoExceptionsDuringBigInsertionReverseOrder) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 100; i > 0; --i) {
        EXPECT_NO_THROW(test.insert(std::move(i)));
    }
}

// HEIGHT Property
TEST(AVLHeight, Small) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 10; i > 0; --i) {
        test.insert(std::move(i));
    }
    test.assert_height_property();
}

TEST(AVLHeight, Big) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 100; i > 0; --i) {
        test.insert(std::move(i));
    }
    test.assert_height_property();
}

// Tree property
TEST(TreeProperty, Small) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 10; i > 0; --i) {
        test.insert(std::move(i));
    }
    test.assert_tree_property();
}

TEST(TreeProperty, Big) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 100; i > 0; --i) {
        test.insert(std::move(i));
    }
    test.assert_tree_property();
}

// REMOVE
TEST(AVLRemove, NoExceptionsSmall) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 0; i < 10; ++i) {
        (test.insert(std::move(i)));
    }
    for (int i = 9; i >= 0; --i) {
        EXPECT_NO_THROW(test.remove(i));
        test.assert_height_property();
        test.assert_tree_property();
    }
}

TEST(AVLRemove, NoExceptionsBig) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 0; i < 100; ++i) {
        (test.insert(std::move(i)));
    }
    for (int i = 99; i >= 0; --i) {
        EXPECT_NO_THROW(test.remove(i));
        test.assert_height_property();
        test.assert_tree_property();
    }
}

TEST(AVLRemove, NoExceptionsSmallReverse) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 9; i >= 0; --i) {
        (test.insert(std::move(i)));
    }
    for (int i = 0; i < 10; ++i) {
        EXPECT_NO_THROW(test.remove(i));
        test.assert_height_property();
        test.assert_tree_property();
    }
}

TEST(AVLRemove, NoExceptionsBigReverse) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 99; i >= 0; --i) {
        (test.insert(std::move(i)));
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_NO_THROW(test.remove(i));
        test.assert_height_property();
        test.assert_tree_property();
    }
}

// OrderStatistic
TEST(AVLOrderStatistics, Small) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 0; i < 10; ++i) {
        (test.insert(std::move(i)));
    }
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(i, test.find(i));
    }
}

TEST(AVLOrderStatistics, SmallReverse) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 0; i < 10; ++i) {
        (test.insert(std::move(i)));
    }
    for (int i = 9; i >= 0; --i) {
        ASSERT_EQ(i, test.find(i));
    }
}

TEST(AVLOrderStatistics, Big) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 0; i < 100; ++i) {
        (test.insert(std::move(i)));
    }
    for (int i = 0; i < 100; ++i) {
        ASSERT_EQ(i, test.find(i));
    }
}

TEST(AVLOrderStatistics, BigReverse) {
    auto test = AVLTreeTest(basic_cmp<int>);
    for (int i = 0; i < 100; ++i) {
        (test.insert(std::move(i)));
    }
    for (int i = 99; i >= 0; --i) {
        ASSERT_EQ(i, test.find(i));
    }
}
