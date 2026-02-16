#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "course.h"

/**
 * @brief Loads courses from a CSV into an ID-index map.
 *
 * Notes:
 * - Normalizes course IDs and prerequisite IDs to uppercase.
 * - Trims whitespace for all fields.
 * - Skips malformed rows safely and records warnings.
 *
 * @param fileName Path to CSV.
 * @param outIndex Output map of courseId -> Course.
 * @param warnings Output warnings (non-fatal).
 * @return true if file opened and at least attempted to parse; false if file open fails.
 */
bool loadCoursesFromCSV(const std::string& fileName, std::unordered_map<std::string, Course>& outIndex, std::vector<std::string>& warnings);
