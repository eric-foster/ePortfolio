//============================================================================
// File        : test_trees.cpp
// Description : Targeted tests for BST and AVL tree behavior.
//
// Purpose:
//   - Validate insert, search, overwrite, and size semantics.
//   - Create a worst-case insertion order to demonstrate behavioral
//     differences between unbalanced BST and self-balancing AVL.
//   - Ensure both implementations produce identical logical results.
//
// Build example (from project root):
//   g++ -std=c++17 -I../include ../src/avl.cpp ../src/benchmark.cpp ../src/bst.cpp ../src/csv_loader.cpp ../src/utils.cpp test_trees.cpp -o test_trees
//
// Run:
//   ./test_trees
//============================================================================

#include <iostream>
#include <vector>
#include <cassert>

#include "avl.h"
#include "bst.h"
#include "utils.h"

/**
 * @brief Helper to construct a normalized Course record.
 *
 * Ensures all test course IDs are uppercased so behavior matches
 * production normalization rules.
 */
static Course makeCourse(const std::string& id, const std::string& title) {
    Course c;
    c.courseId = toUpper(id);
    c.title = title;
    return c;
}

int main() {
    {
        BstTree bst;
        AvlTree avl;

        // Insert in strictly increasing order to intentionally produce
        // a worst-case (skewed) BST shape while AVL remains balanced.
        std::vector<Course> courses = {
            makeCourse("CS100", "Intro"),
            makeCourse("CS200", "Intermediate"),
            makeCourse("CS300", "Advanced"),
            makeCourse("CS400", "Capstone")
        };

        for (const auto& c : courses) {
            bst.insert(c);
            avl.insert(c);
        }

        // Size should reflect number of unique keys.
        assert(bst.size() == 4);
        assert(avl.size() == 4);

        // Successful searches.
        assert(bst.search("CS300") != nullptr);
        assert(avl.search("CS300") != nullptr);

        // Unsuccessful searches.
        assert(bst.search("CS999") == nullptr);
        assert(avl.search("CS999") == nullptr);

        // Duplicate-key overwrite behavior:
        // inserting same courseId should update title without changing size.
        Course updated = makeCourse("CS200", "Intermediate II");
        bst.insert(updated);
        avl.insert(updated);

        assert(bst.search("CS200")->title == "Intermediate II");
        assert(avl.search("CS200")->title == "Intermediate II");
    }

    std::cout << "All tests passed.\n";
    return 0;
}
