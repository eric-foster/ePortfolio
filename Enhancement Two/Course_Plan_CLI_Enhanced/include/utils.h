#pragma once
#include <string>
#include <vector>

/**
 * @brief Trim leading/trailing whitespace.
 */
std::string trim(const std::string& s);

/**
 * @brief Convert a string to uppercase (ASCII).
 */
std::string toUpper(std::string s);

/**
 * @brief Split a simple comma-delimited CSV line (no quoted fields expected).
 */
std::vector<std::string> splitCSVLine(const std::string& line);
