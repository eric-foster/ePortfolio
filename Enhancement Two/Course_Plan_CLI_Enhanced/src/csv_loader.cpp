#include "csv_loader.h"
#include "utils.h"
#include <fstream>

/**
 * @file csv_loader.cpp
 * @brief CSV parsing and course-loading utilities.
 *
 * Expected CSV format (per line):
 *   CourseId,Title,Prereq1,Prereq2,...
 *
 * Design goals:
 * - Be tolerant of minor formatting issues (extra whitespace, empty fields).
 * - Skip malformed rows but continue processing remaining data.
 * - Normalize course identifiers to uppercase for consistent comparisons.
 */

/**
 * @brief Load course records from a CSV file into an index map.
 *
 * Each valid row produces a Course object keyed by courseId. If a duplicate
 * courseId is encountered, the later entry overwrites the earlier one.
 *
 * Error handling strategy:
 * - File open failure returns false and records an error message.
 * - Malformed rows are skipped and reported in the warnings vector.
 *
 * @param fileName Path to CSV file.
 * @param outIndex Map of courseId -> Course to populate.
 * @param warnings Output vector of warnings/errors.
 * @return true if file was opened successfully, false otherwise.
 */
bool loadCoursesFromCSV(const std::string& fileName, std::unordered_map<std::string, Course>& outIndex, std::vector<std::string>& warnings) {
    std::ifstream inputFile(fileName);

    // Fail fast if file cannot be opened.
    if (!inputFile.is_open()) {
        warnings.push_back(
            "ERROR: File could not be opened. Ensure the CSV is in the working directory."
        );
        return false;
    }

    std::string line;
    size_t lineNo = 0;

    // Read file line-by-line to allow precise warning messages.
    while (std::getline(inputFile, line)) {
        lineNo++;

        // Remove leading/trailing whitespace before further processing.
        line = trim(line);

        // Skip empty or whitespace-only lines.
        if (line.empty()) continue;

        // Split respecting quoted fields (handled by splitCSVLine).
        std::vector<std::string> fields = splitCSVLine(line);

        // At minimum we expect CourseId and Title.
        if (fields.size() < 2) {
            warnings.push_back("Line " + std::to_string(lineNo) + ": invalid format (expected at least CourseId,Title). Skipped.");
            continue;
        }

        Course c;

        // Normalize course IDs to uppercase so lookups are case-insensitive.
        c.courseId = toUpper(trim(fields[0]));
        c.title = trim(fields[1]);

        // Validate required fields.
        if (c.courseId.empty() || c.title.empty()) {
            warnings.push_back( "Line " + std::to_string(lineNo) + ": missing CourseId or Title. Skipped.");
            continue;
        }

        // Remaining fields (if any) are prerequisite course IDs.
        for (size_t i = 2; i < fields.size(); i++) {
            std::string prereqId = toUpper(trim(fields[i]));

            // Ignore empty prereq entries to tolerate trailing commas.
            if (!prereqId.empty()) {
                c.prereq.push_back(prereqId);
            }
        }

        // Insert or overwrite by courseId.
        // Using the map as an index allows O(1) average lookup and update.
        outIndex[c.courseId] = c;
    }

    return true;
}
