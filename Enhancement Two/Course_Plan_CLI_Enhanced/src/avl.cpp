#include "avl.h"
#include <algorithm>

/**
 * @file avl.cpp
 * @brief Implementation of the AvlTree class.
 *
 * Important invariants:
 * - BST ordering by Course::courseId is maintained.
 * - AVL balance invariant is maintained after every insertion:
 *     |balanceFactor(node)| <= 1
 *   through single or double rotations.
 */

AvlTree::AvlTree() : root_(nullptr), count_(0) {}

AvlTree::~AvlTree() {
    // Free all nodes and reset internal state.
    destroy(root_);
    root_ = nullptr;
    count_ = 0;
}

void AvlTree::destroy(Node* node) {
    if (!node) return;

    // Post-order deletion: free children first, then the parent.
    destroy(node->left);
    destroy(node->right);
    delete node;
}

int AvlTree::h(Node* n) {
    // Treat null as height 0 so leaf height can be 1.
    return n ? n->height : 0;
}

void AvlTree::updateHeight(Node* n) {
    if (!n) return;

    // Height depends only on child subtree heights.
    n->height = 1 + std::max(h(n->left), h(n->right));
}

int AvlTree::balanceFactor(Node* n) {
    // Positive => left-heavy; negative => right-heavy.
    return n ? (h(n->left) - h(n->right)) : 0;
}

AvlTree::Node* AvlTree::rotateRight(Node* y) {
    // Right rotation is used to fix a left-left imbalance.
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation (rewire pointers).
    x->right = y;
    y->left = T2;

    // Heights must be updated bottom-up after pointer changes.
    updateHeight(y);
    updateHeight(x);

    // x becomes the new root of this subtree.
    return x;
}

AvlTree::Node* AvlTree::rotateLeft(Node* x) {
    // Left rotation is used to fix a right-right imbalance.
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation (rewire pointers).
    y->left = x;
    x->right = T2;

    // Heights must be updated bottom-up after pointer changes.
    updateHeight(x);
    updateHeight(y);

    // y becomes the new root of this subtree.
    return y;
}

AvlTree::Node* AvlTree::rebalance(Node* node) {
    // Recompute height first; balance factor depends on up-to-date heights.
    updateHeight(node);
    int bf = balanceFactor(node);

    // Left heavy: bf > 1
    if (bf > 1) {
        // If left child is right-heavy, it's a Left-Right (LR) case:
        // rotate left on child first, then rotate right on node.
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left); // LR step 1
        }
        return rotateRight(node); // LL or LR step 2
    }

    // Right heavy: bf < -1
    if (bf < -1) {
        // If right child is left-heavy, it's a Right-Left (RL) case:
        // rotate right on child first, then rotate left on node.
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right); // RL step 1
        }
        return rotateLeft(node); // RR or RL step 2
    }

    // Already balanced.
    return node;
}

AvlTree::Node* AvlTree::insertRec(Node* node, const Course& course, bool& inserted) {
    // Standard BST insert, followed by rebalancing on the way back up.
    if (!node) {
        inserted = true;              // A new unique key increases size.
        return new Node(course);
    }

    if (course.courseId < node->course.courseId) {
        node->left = insertRec(node->left, course, inserted);
    } else if (course.courseId > node->course.courseId) {
        node->right = insertRec(node->right, course, inserted);
    } else {
        // Duplicate ID: overwrite stored record to keep key uniqueness.
        // Note: size (count_) does not change in this case.
        node->course = course;
        inserted = false;
        return node; // No need to rebalance since tree structure didn't change.
    }

    // Rebalance ensures AVL invariant holds after subtree insertion.
    return rebalance(node);
}

void AvlTree::insert(const Course& course) {
    bool inserted = false;
    root_ = insertRec(root_, course, inserted);

    // Only increment count on first-time insert (unique key).
    if (inserted) count_++;
}

const AvlTree::Node* AvlTree::searchRec(const Node* node, const std::string& courseId) {
    // Iterative search is used here to avoid recursion overhead for lookups.
    const Node* curr = node;

    while (curr) {
        if (courseId == curr->course.courseId) return curr;

        // Follow BST ordering based on key comparison.
        curr = (courseId < curr->course.courseId) ? curr->left : curr->right;
    }

    return nullptr;
}

const Course* AvlTree::search(const std::string& courseId) const {
    const Node* n = searchRec(root_, courseId);
    return n ? &n->course : nullptr;
}

void AvlTree::inOrderRec(const Node* node, std::vector<const Course*>& out) {
    if (!node) return;

    // In-order traversal yields sorted order by key.
    inOrderRec(node->left, out);
    out.push_back(&node->course);
    inOrderRec(node->right, out);
}

void AvlTree::inOrder(std::vector<const Course*>& out) const {
    // Caller controls whether to clear out; we append for flexibility.
    inOrderRec(root_, out);
}

size_t AvlTree::size() const {
    return count_;
}
