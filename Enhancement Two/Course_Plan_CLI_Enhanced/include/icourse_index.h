#pragma once
#include <string>
#include "course.h"

// Common interface so BST and AVL can be benchmarked and tested consistently.
class ICourseIndex {
public:
    virtual ~ICourseIndex() = default;

    // Insert or overwrite a course by courseId.
    virtual void insert(const Course& c) = 0;

    // Returns nullptr if not found.
    virtual const Course* find(const std::string& courseId) const = 0;

    // In-order print to stdout.
    virtual void printInOrder() const = 0;

    virtual size_t size() const = 0;
};
