#include "utils.h"
#include <cctype>
#include <sstream>

/**
 * @file utils.cpp
 * @brief String utility helpers used across the application.
 *
 * These functions centralize common string operations such as trimming,
 * case normalization, and CSV tokenization so that parsing and comparison
 * behavior is consistent across modules.
 */

/**
 * @brief Remove leading and trailing whitespace from a string.
 *
 * Whitespace is defined using std::isspace and therefore includes spaces,
 * tabs, newlines, and other locale-independent whitespace characters.
 *
 * @param s Input string.
 * @return Trimmed copy of the input string.
 */
std::string trim(const std::string& s) {
    size_t start = 0;

    // Advance start index while characters are whitespace.
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }

    size_t end = s.size();

    // Move end index backward while characters are whitespace.
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }

    // Substring between first and last non-whitespace characters.
    return s.substr(start, end - start);
}

/**
 * @brief Convert all characters in a string to uppercase.
 *
 * Used to normalize course identifiers so comparisons are case-insensitive.
 *
 * @param s Input string (copied).
 * @return Uppercase version of the input string.
 */
std::string toUpper(std::string s) {
    for (char& ch : s) {
        // Cast to unsigned char before toupper to avoid undefined behavior.
        ch = static_cast<char>(
            std::toupper(static_cast<unsigned char>(ch))
        );
    }
    return s;
}

/**
 * @brief Split a CSV line into individual fields.
 *
 * Fields are separated by commas. Each token is trimmed of leading and
 * trailing whitespace. This implementation does not currently support
 * embedded commas inside quoted fields.
 *
 * @param line Raw CSV line.
 * @return Vector of trimmed field strings.
 */
std::vector<std::string> splitCSVLine(const std::string& line) {
    std::vector<std::string> out;
    std::string token;

    // Use stringstream to process comma-delimited fields.
    std::stringstream ss(line);

    while (std::getline(ss, token, ',')) {
        // Trim each token to tolerate extra whitespace.
        out.push_back(trim(token));
    }

    return out;
}
