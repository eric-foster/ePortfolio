#include "bst.h"

/**
 * @file bst.cpp
 * @brief Implementation of the BstTree class.
 *
 * This file provides a straightforward, non-balancing BST implementation.
 * It intentionally avoids rotations or height tracking so it can serve as
 * a performance baseline when compared with the AVL tree.
 */

BstTree::BstTree() : root_(nullptr), count_(0) {}

BstTree::~BstTree() {
    // Recursively delete all nodes and reset internal state.
    destroy(root_);
    root_ = nullptr;
    count_ = 0;
}

void BstTree::destroy(Node* node) {
    if (!node) return;

    // Post-order deletion ensures children are freed first.
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void BstTree::insert(const Course& course) {
    // Special case: empty tree.
    if (!root_) {
        root_ = new Node(course);
        count_++;
        return;
    }

    // Iterative descent avoids recursion overhead.
    Node* curr = root_;

    while (curr) {
        if (course.courseId < curr->course.courseId) {
            if (!curr->left) {
                curr->left = new Node(course);
                count_++;
                return;
            }
            curr = curr->left;
        } else if (course.courseId > curr->course.courseId) {
            if (!curr->right) {
                curr->right = new Node(course);
                count_++;
                return;
            }
            curr = curr->right;
        } else {
            // Duplicate key:
            // Overwrite existing data but do not increase size.
            curr->course = course;
            return;
        }
    }
}

const Course* BstTree::search(const std::string& courseId) const {
    // Iterative lookup following BST ordering.
    const Node* curr = root_;

    while (curr) {
        if (courseId == curr->course.courseId) {
            return &curr->course;
        }

        // Traverse left or right based on key comparison.
        curr = (courseId < curr->course.courseId) ? curr->left : curr->right;
    }

    return nullptr;
}

void BstTree::inOrderRec(const Node* node, std::vector<const Course*>& out) {
    if (!node) return;

    // Left -> Node -> Right produces sorted order.
    inOrderRec(node->left, out);
    out.push_back(&node->course);
    inOrderRec(node->right, out);
}

void BstTree::inOrder(std::vector<const Course*>& out) const {
    // Caller controls whether 'out' is cleared beforehand.
    inOrderRec(root_, out);
}

size_t BstTree::size() const {
    return count_;
}
