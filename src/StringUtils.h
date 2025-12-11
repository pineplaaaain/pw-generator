#pragma once

#include <algorithm>
#include <string>

class StringUtils {
 public:
  static std::string toupper(const std::string &str) {
    std::string upper = str;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    return upper;
  }

  static std::string tolower(const std::string &str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
  }
};