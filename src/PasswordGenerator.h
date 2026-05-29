#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "PasswordCharset.h"

class PasswordGenerator {
 public:
  static std::string generate(size_t length, const std::string& charset);
  static bool ask(const PasswordCharset& charset);

  // Generate with guaranteed minimum 1 char from each selected charset
  static std::string generateGuaranteed(
      size_t length, const std::vector<std::string>& charsets);
};

inline std::string PasswordGenerator::generate(size_t length,
                                               const std::string& charset) {
  if (charset.empty()) return "";

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dis(0, charset.size() - 1);

  std::string password;
  password.reserve(length);

  for (size_t i = 0; i < length; ++i) {
    password += charset[dis(gen)];
  }

  return password;
}

inline std::string PasswordGenerator::generateGuaranteed(
    size_t length, const std::vector<std::string>& charsets) {
  if (charsets.empty()) return "";
  if (length < charsets.size()) return "";

  std::random_device rd;
  std::mt19937 gen(rd());

  std::string password;
  password.reserve(length);

  // Step 1: Pick 1 random character from each charset
  for (const auto& charset : charsets) {
    if (charset.empty()) return "";
    std::uniform_int_distribution<size_t> dis(0, charset.size() - 1);
    password += charset[dis(gen)];
  }

  // Step 2: Build combined pool and fill remaining positions
  std::string combined;
  for (const auto& charset : charsets) {
    combined += charset;
  }
  std::uniform_int_distribution<size_t> dis(0, combined.size() - 1);

  for (size_t i = charsets.size(); i < length; ++i) {
    password += combined[dis(gen)];
  }

  // Step 3: Fisher-Yates shuffle
  for (size_t i = password.size() - 1; i > 0; --i) {
    std::uniform_int_distribution<size_t> swap_dis(0, i);
    size_t j = swap_dis(gen);
    std::swap(password[i], password[j]);
  }

  return password;
}

inline bool PasswordGenerator::ask(const PasswordCharset& charset) {
  char input;
  std::cout << charset.message << " -- " << charset.str << " (y/n)?: ";
  std::cin >> input;

  while (input != 'y' && input != 'n') {
    std::cout << "Invalid input. Please enter 'y' or 'n': ";
    std::cin >> input;
  }

  return input == 'y';
}