#pragma once
#include <string>
#include <vector>
#include "tree_iface.h"

/**
 * @brief Runs a repeated-search benchmark against a tree implementation.
 *
 * @param tree Tree implementation (BST or AVL).
 * @param ids Course IDs to search (already normalized).
 * @param iterationsPerId Number of searches per id.
 * @param label Label for printing (e.g., "BST" or "AVL").
 */
void runSearchBenchmark(const ICourseTree& tree, const std::vector<std::string>& ids, size_t iterationsPerId, const std::string& label);
