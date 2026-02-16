//============================================================================
// Name        : Course Planner (Modular Enhanced)
// Author      : Eric Foster
// Version     : 2.0
// Description : Modular CS 300 artifact enhanced for CS 499.
//               - CSV parsing hardened (trim, normalize, safe skipping)
//               - Modular design for testability (separate tree + loader)
//               - Benchmark compares BST vs AVL search performance
//============================================================================

/**
 * @file main.cpp
 * @brief CLI entry point for the modular advising tool.
 *
 * Program overview:
 * - Option 1 loads courses from CSV into an index and then inserts them into both BST and AVL.
 * - Options 2–3 use the AVL tree to print schedule and course details (sorted traversal + search).
 * - Option 4 runs a controlled benchmark comparing BST vs. AVL search performance using identical
 *   IDs and iteration counts for apples-to-apples results.
 *
 * Design notes:
 * - A hash index (unordered_map) is used as a staging structure after CSV load so we can:
 *   (1) validate/normalize input once, and
 *   (2) populate multiple tree implementations consistently.
 * - AVL is used for schedule printing because in-order traversal yields sorted output by courseId.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

#include "avl.h"
#include "benchmark.h"
#include "bst.h"
#include "csv_loader.h"
#include "utils.h"

using namespace std;

/**
 * @brief Print a single course record in a user-friendly format.
 *
 * This function centralizes formatting for course details so the menu handler
 * stays focused on control flow and validation.
 *
 * @param course Course record to print.
 */
static void printCourseDetails(const Course& course) {
    cout << course.courseId << ", " << course.title << "\n";
    cout << "Prerequisites: ";

    // Printing prerequisites requires careful formatting to avoid trailing spaces.
    if (course.prereq.empty()) {
        cout << "No prerequisites";
    } else {
        for (size_t i = 0; i < course.prereq.size(); i++) {
            cout << course.prereq[i] << (i + 1 < course.prereq.size() ? " " : "");
        }
    }

    cout << "\n\n";
}

/**
 * @brief Read a menu choice from stdin with input validation.
 *
 * This protects the main loop from entering a failure state if the user types
 * non-numeric input ("abc"). We clear the failbit and discard the line.
 *
 * @return Integer menu choice as entered by the user.
 */
static int readMenuChoice() {
    int choice;
    cout << "Enter selection: ";

    while (!(cin >> choice)) {
        cout << "Not a valid input. Try a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter selection: ";
    }

    return choice;
}

/**
 * @brief Read a size_t value (non-negative integer) from stdin with validation.
 *
 * Used for benchmark iteration counts. If the user provides invalid input,
 * the stream is recovered and the user is prompted again.
 *
 * @param prompt Prompt string displayed to the user.
 * @return Parsed size_t value.
 */
static size_t readPositiveSizeT(const string& prompt) {
    cout << prompt;

    size_t value = 0;
    while (!(cin >> value)) {
        cout << "Not a valid input. Enter a positive integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << prompt;
    }

    return value;
}

int main(int argc, char* argv[]) {
    // Input file can be specified as a single CLI argument for convenience.
    // Default assumes the CSV is in the working directory.
    string csvPath = (argc == 2) ? argv[1] : "CS 300 ABCU_Advising_Program_Input.csv";

    // Index map used as a canonical source of course records after parsing.
    // Key is normalized courseId (uppercase).
    unordered_map<string, Course> index;

    // Warnings are collected during parsing to keep load resilient:
    // invalid/malformed rows can be skipped without aborting the whole load.
    vector<string> warnings;

    // Sorted list of normalized IDs used by the benchmark to ensure deterministic workload.
    vector<string> ids;

    // Two trees built from identical input for apples-to-apples comparison.
    // BST provides a baseline; AVL provides worst-case O(log n) guarantees.
    BstTree bst;
    AvlTree avl;

    bool loaded = false;

    cout << "Welcome to Course Planner.\n";

    int choice = 0;
    while (choice != 9) {
        cout << "\nMenu:\n";
        cout << "  1. Load Courses\n";
        cout << "  2. Print Schedule (AVL)\n";
        cout << "  3. Print Course (AVL)\n";
        cout << "  4. Benchmark Search (BST vs AVL)\n";
        cout << "  9. Exit\n";

        choice = readMenuChoice();

        if (choice == 1) {
            // Reset all runtime state so re-loading produces clean, repeatable behavior.
            // Note: bst/avl objects are not reset here; if you add "reload" support later,
            // consider reconstructing trees or adding clear() methods for correctness.
            index.clear();
            warnings.clear();
            ids.clear();

            loaded = loadCoursesFromCSV(csvPath, index, warnings);
            if (!loaded) {
                // File open failure (or other fatal parse condition) is surfaced here.
                for (const auto& w : warnings) cout << w << "\n";
                continue;
            }

            // Insert parsed courses into both trees.
            // Inserting from the same canonical index ensures consistent datasets.
            for (const auto& kv : index) {
                bst.insert(kv.second);
                avl.insert(kv.second);
            }

            // Build a stable ID list for benchmarking (sorted to reduce run-to-run variance).
            ids.reserve(index.size());
            for (const auto& kv : index) ids.push_back(kv.first);
            sort(ids.begin(), ids.end());

            cout << avl.size() << " courses read\n";
            for (const auto& w : warnings) cout << w << "\n";
        }
        else if (choice == 2) {
            if (!loaded) {
                cout << "Please load courses first (option 1).\n";
                continue;
            }

            // AVL in-order traversal yields sorted schedule output by courseId.
            vector<const Course*> ordered;
            avl.inOrder(ordered);

            cout << "\nHere is a sample schedule:\n\n";
            for (const Course* c : ordered) {
                cout << c->courseId << ": " << c->title << " | ";

                if (c->prereq.empty()) {
                    cout << "No prerequisites";
                } else {
                    for (size_t i = 0; i < c->prereq.size(); i++) {
                        cout << c->prereq[i] << (i + 1 < c->prereq.size() ? " " : "");
                    }
                }

                cout << "\n";
            }
        }
        else if (choice == 3) {
            if (!loaded) {
                cout << "Please load courses first (option 1).\n";
                continue;
            }

            cout << "What course do you want to know about?\n";
            string input;
            cin >> input;

            // Normalize user input to match loader normalization rules.
            // This prevents case mismatches (e.g., "cs300" vs "CS300").
            string courseId = toUpper(trim(input));

            const Course* found = avl.search(courseId);

            if (found) {
                printCourseDetails(*found);
            } else {
                cout << "Course Id " << courseId << " not found.\n\n";
            }
        }
        else if (choice == 4) {
            if (!loaded) {
                cout << "Please load courses first (option 1).\n";
                continue;
            }

            // Large iteration counts reduce timing noise and produce more stable averages.
            size_t iters = readPositiveSizeT("Iterations per courseId (e.g., 10000): ");

            // Run the same workload against both trees for a fair comparison.
            runSearchBenchmark(bst, ids, iters, "BST");
            runSearchBenchmark(avl, ids, iters, "AVL");
        }
        else if (choice == 9) {
            // User exit: loop condition will end.
        }
        else {
            cout << choice << " is not a valid menu option. Try again.\n";
        }
    }

    cout << "\nThank you for using the course planner. See you soon.\n";
    return 0;
}
