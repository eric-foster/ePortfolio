#include "benchmark.h"
#include <chrono>
#include <iomanip>
#include <iostream>

/**
 * @file benchmark.cpp
 * @brief Micro-benchmark utilities for comparing ICourseTree implementations.
 *
 * This benchmark is intentionally simple and repeatable:
 * - Uses an identical workload (same course IDs, same iteration count) for each tree.
 * - Measures total elapsed time for repeated search operations.
 * - Computes average time per search operation in microseconds.
 *
 * Notes on benchmarking:
 * - Use a large iterationsPerId to reduce timing noise and improve measurement stability.
 */

/**
 * @brief Run a search benchmark against a given tree implementation.
 *
 * Workload:
 * - For each course ID in `ids`, repeatedly call tree.search(id) iterationsPerId times.
 * - Measures total time across the entire workload and reports an average per search.
 *
 * Fairness/consistency:
 * - The caller should ensure both trees are loaded with the same dataset and that `ids`
 *   is the same list used across implementations.
 *
 * @param tree Tree implementation under test (BST or AVL).
 * @param ids List of course IDs to query (the workload).
 * @param iterationsPerId Number of repeated searches per ID (must be >= 1).
 * @param label Output label for clarity in console results.
 */
void runSearchBenchmark(const ICourseTree& tree, const std::vector<std::string>& ids, size_t iterationsPerId, const std::string& label) {
    // Guard against an empty workload: avoids divide-by-zero and meaningless timing.
    if (ids.empty()) {
        std::cout << "Benchmark: no course IDs loaded.\n";
        return;
    }

    // Normalize iteration count to at least 1 so the benchmark always performs work.
    if (iterationsPerId == 0) iterationsPerId = 1;

    using clock = std::chrono::high_resolution_clock;

    // Total number of search calls performed (used for average computation).
    size_t totalOps = 0;

    // Anti-optimization sink:
    // Without this, an optimizing compiler could potentially remove the search calls
    // if it concludes the results are never observed. By folding the returned pointer
    // into a volatile value, we force the loop to have an observable side effect.
    volatile std::uintptr_t sink = 0;

    // Start timing as close as possible to the workload loop to avoid measuring setup.
    auto start = clock::now();

    for (const auto& id : ids) {
        for (size_t i = 0; i < iterationsPerId; i++) {
            // Search returns a pointer to an internal Course if found, otherwise nullptr.
            const Course* found = tree.search(id);

            // Mix the pointer value into the sink to prevent dead code elimination.
            sink ^= reinterpret_cast<std::uintptr_t>(found);

            totalOps++;
        }
    }

    auto end = clock::now();

    // Measure microseconds to preserve resolution for very fast operations.
    // Using milliseconds can lead to "0.000" averages for tight loops.
    auto totalUs =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Average time per search operation in microseconds.
    double avgUs = static_cast<double>(totalUs) / static_cast<double>(totalOps);

    // Fixed formatting improves readability and makes side-by-side comparison easier.
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "\n" << label << " Benchmark Results\n";
    std::cout << "  Total searches: " << totalOps << "\n";
    std::cout << "  Total time (us): " << totalUs << "\n";
    std::cout << "  Avg time per search (us): " << avgUs << "\n";

    // Consume sink so the compiler can't treat it as unused.
    if (sink == 0xFFFFFFFF) std::cout << "";
}
