//============================================================================
// File        : smoke_test.cpp
// Description : Minimal smoke test (no test framework) to validate core behavior
//               of BST and AVL tree implementations.
//
// Purpose:
//   - Verify that basic insert and search operations work.
//   - Confirm consistent behavior between BST and AVL implementations.
//   - Catch obvious regressions before running the main application.
//
// Build example (from project root):
//   g++ -std=c++17 -I../include ../src/avl.cpp ../src/benchmark.cpp ../src/bst.cpp ../src/csv_loader.cpp ../src/utils.cpp smoke_test.cpp -o smoke_test
//
// Run:
//   ./smoke_test
//============================================================================

#include <cassert>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include "bst.h"
#include "avl.h"
#include "course.h"

/**
 * @brief Entry point for smoke test executable.
 *
 * This is intentionally lightweight and does not rely on a testing framework.
 * The goal is fast feedback that critical paths (insert/search) still function.
 */
int main() {
    // Construct a small, deterministic dataset.
    Course a{"CS100", "Intro", {}};
    Course b{"CS200", "DSA", {"CS100"}};
    Course c{"CS150", "Foundations", {}};

    // Create both tree implementations.
    BstTree bst;
    AvlTree avl;

    // Insert identical data into both trees.
    bst.insert(a);
    bst.insert(b);
    bst.insert(c);

    avl.insert(a);
    avl.insert(b);
    avl.insert(c);

    // Validate successful searches.
    assert(bst.search("CS200") != nullptr);
    assert(avl.search("CS200") != nullptr);

    // Validate unsuccessful searches.
    assert(bst.search("NOPE") == nullptr);
    assert(avl.search("NOPE") == nullptr);

    std::cout << "Smoke test passed.\n";

    return 0;
}
