#pragma once

#include <iostream>
#include <random>
#include <string>

#include "PasswordCharset.h"

class PasswordGenerator {
 public:
  static std::string generate(size_t length, const std::string& charset);
  static bool ask(const PasswordCharset& charset);
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