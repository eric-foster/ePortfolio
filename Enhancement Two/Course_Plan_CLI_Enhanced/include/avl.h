#pragma once

#include "tree_iface.h"

/**
 * @file avl.h
 * @brief AVL tree implementation of the ICourseTree interface.
 *
 * This module provides a self-balancing binary search tree (AVL Tree) for storing
 * and searching Course records by courseId. Balancing is maintained via rotations
 * after insertions, ensuring O(log n) worst-case search/insert time.
 *
 * Design note:
 * - This class is intentionally used alongside a plain BST implementation to
 *   support benchmarking and empirical performance comparison.
 */
class AvlTree : public ICourseTree {
public:
    /**
     * @brief Construct an empty AVL tree.
     */
    AvlTree();

    /**
     * @brief Destroy the tree and free all allocated nodes.
     */
    ~AvlTree() override;

    // Non-copyable: tree nodes are heap allocated and owned by this instance.
    AvlTree(const AvlTree&) = delete;
    AvlTree& operator=(const AvlTree&) = delete;

    /**
     * @brief Insert a course keyed by courseId.
     *
     * If a course with the same courseId already exists, it is overwritten
     * (the size() does not change).
     *
     * @param course Course record to insert.
     * @complexity O(log n) worst-case due to AVL rebalancing.
     */
    void insert(const Course& course) override;

    /**
     * @brief Search for a course by courseId.
     *
     * @param courseId Key used for lookup (typically normalized by caller if required).
     * @return Pointer to the stored Course if found; otherwise nullptr.
     * @complexity O(log n) worst-case.
     */
    const Course* search(const std::string& courseId) const override;

    /**
     * @brief In-order traversal of all courses.
     *
     * Produces results sorted by courseId (BST ordering).
     *
     * @param out Output vector of pointers to Course objects stored in the tree.
     * @post out is appended to (caller may want to clear it first).
     * @complexity O(n).
     */
    void inOrder(std::vector<const Course*>& out) const override;

    /**
     * @brief Number of unique courseId entries stored in the tree.
     *
     * Duplicate inserts overwrite the existing entry and do not change the count.
     */
    size_t size() const override;

private:
    /**
     * @brief Internal AVL node representation.
     *
     * height is maintained as:
     *   height(node) = 1 + max(height(left), height(right))
     * with empty child height = 0.
     */
    struct Node {
        Course course;          ///< Stored course record.
        Node* left = nullptr;   ///< Left subtree (keys < this->course.courseId).
        Node* right = nullptr;  ///< Right subtree (keys > this->course.courseId).
        int height = 1;         ///< Height of subtree rooted at this node (leaf = 1).

        explicit Node(const Course& c) : course(c) {}
    };

    Node* root_;     ///< Root node of the AVL tree (nullptr if empty).
    size_t count_;   ///< Number of unique keys currently stored.

    /**
     * @brief Recursively delete nodes in a post-order traversal.
     *
     * Post-order deletion ensures children are freed before their parent,
     * avoiding use-after-free on child pointers.
     */
    static void destroy(Node* node);

    /**
     * @brief Safe height accessor (returns 0 for null).
     */
    static int h(Node* n);

    /**
     * @brief Update node height from children heights.
     */
    static void updateHeight(Node* n);

    /**
     * @brief Compute balance factor: height(left) - height(right).
     *
     * AVL invariant requires |balanceFactor(node)| <= 1 for all nodes.
     */
    static int balanceFactor(Node* n);

    /**
     * @brief Perform a left rotation.
     *
     * Rotates subtree rooted at x to the left
     *
     * @return New root of rotated subtree.
     */
    static Node* rotateLeft(Node* x);

    /**
     * @brief Perform a right rotation.
     *
     * Rotates subtree rooted at y to the right
     *
     * @return New root of rotated subtree.
     */
    static Node* rotateRight(Node* y);

    /**
     * @brief Restore AVL balance at the given node, returning new subtree root.
     *
     * Called after insertion into either subtree.
     */
    static Node* rebalance(Node* node);

    /**
     * @brief Recursive insert that returns the (possibly new) subtree root.
     *
     * @param node Subtree root to insert into.
     * @param course Course record to insert.
     * @param inserted Set to true only when a new node is created (unique key).
     * @return New subtree root after insertion and rebalancing.
     */
    static Node* insertRec(Node* node, const Course& course, bool& inserted);

    /**
     * @brief Iterative search within a subtree.
     *
     * @return Pointer to node if found, else nullptr.
     */
    static const Node* searchRec(const Node* node, const std::string& courseId);

    /**
     * @brief Recursive in-order traversal helper.
     */
    static void inOrderRec(const Node* node, std::vector<const Course*>& out);
};
