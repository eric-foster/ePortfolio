#pragma once
#include <string>
#include <vector>

/**
 * @brief Represents a single course record loaded from the advising CSV.
 */
struct Course {
    std::string courseId;               // "CS200"
    std::string title;                  // "Data Structures"
    std::vector<std::string> prereq;    // optional prerequisites (course IDs)
};
