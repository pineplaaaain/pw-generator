#pragma once

#include <string>

struct PasswordCharset {
  std::string message;
  std::string str;
};

// Canonical charset definitions used across interactive and one-shot modes
namespace charsets {
inline const std::string LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
inline const std::string UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
inline const std::string NUMBERS = "0123456789";
inline const std::string SYMBOLS = "!#$%^&*()_+-=";
}  // namespace charsets