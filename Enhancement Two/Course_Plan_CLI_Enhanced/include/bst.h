#pragma once

#include "tree_iface.h"

/**
 * @file bst.h
 * @brief Unbalanced Binary Search Tree (BST) implementation of ICourseTree.
 *
 * This implementation maintains standard BST ordering by Course::courseId.
 * No self-balancing is performed.
 *
 * Performance characteristics:
 * - Average-case insert/search: O(log n)
 * - Worst-case insert/search: O(n) when tree becomes skewed
 *
 * This class exists primarily to provide a baseline for comparison
 * against the self-balancing AVL implementation.
 */
class BstTree : public ICourseTree {
public:
    /**
     * @brief Construct an empty BST.
     */
    BstTree();

    /**
     * @brief Destroy the tree and free all allocated nodes.
     */
    ~BstTree() override;

    // Non-copyable to prevent accidental shallow copies of node pointers.
    BstTree(const BstTree&) = delete;
    BstTree& operator=(const BstTree&) = delete;

    /**
     * @brief Insert a course keyed by courseId.
     *
     * If a duplicate courseId is encountered, the existing record
     * is overwritten and the tree size does not change.
     *
     * @param course Course record to insert.
     * @complexity Average O(log n), worst-case O(n).
     */
    void insert(const Course& course) override;

    /**
     * @brief Search for a course by courseId.
     *
     * @param courseId Key used for lookup.
     * @return Pointer to stored Course if found; otherwise nullptr.
     * @complexity Average O(log n), worst-case O(n).
     */
    const Course* search(const std::string& courseId) const override;

    /**
     * @brief In-order traversal of the tree.
     *
     * Produces results sorted by courseId.
     *
     * @param out Output vector of pointers to Course objects.
     * @complexity O(n).
     */
    void inOrder(std::vector<const Course*>& out) const override;

    /**
     * @brief Number of unique course records stored.
     */
    size_t size() const override;

private:
    /**
     * @brief Internal BST node representation.
     */
    struct Node {
        Course course;          //< Stored course record.
        Node* left = nullptr;   //< Left child (keys < courseId).
        Node* right = nullptr;  //< Right child (keys > courseId).

        explicit Node(const Course& c) : course(c) {}
    };

    Node* root_;     //< Root node of the BST (nullptr if empty).
    size_t count_;   //< Number of unique keys stored.

    /**
     * @brief Recursively delete nodes in post-order.
     *
     * Children are deleted before parents to avoid invalid access.
     */
    static void destroy(Node* node);

    /**
     * @brief Recursive in-order traversal helper.
     */
    static void inOrderRec(const Node* node, std::vector<const Course*>& out);
};
