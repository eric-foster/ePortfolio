#pragma once
#include <string>
#include <vector>
#include "course.h"

/**
 * @brief Minimal search-tree interface to support swapping implementations
 *        (BST vs AVL) for benchmarking and testing.
 */
class ICourseTree {
public:
    virtual ~ICourseTree() = default;

    virtual void insert(const Course& course) = 0;
    virtual const Course* search(const std::string& courseId) const = 0;
    virtual void inOrder(std::vector<const Course*>& out) const = 0;
    virtual size_t size() const = 0;
};
