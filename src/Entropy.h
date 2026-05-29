#pragma once

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

class Entropy {
 public:
  // Calculate entropy in bits: log2(charset_size) * length
  static double calculate(size_t charsetSize, size_t length) {
    if (charsetSize <= 1) return 0.0;
    return std::log2(static_cast<double>(charsetSize)) *
           static_cast<double>(length);
  }

  // Get a human-readable strength label
  static std::string strengthLabel(double bits) {
    if (bits < 40) return "Weak";
    if (bits < 60) return "Fair";
    if (bits < 80) return "Strong";
    if (bits < 100) return "Very Strong";
    return "Excellent";
  }

  // Format entropy info as a display string
  static std::string format(size_t charsetSize, size_t length) {
    double bits = calculate(charsetSize, length);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << bits << " bits ("
        << strengthLabel(bits) << ")";
    return oss.str();
  }
};
