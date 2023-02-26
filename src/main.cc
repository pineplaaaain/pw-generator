#include <iostream>
#include <random>
#include <string>

#include "PasswordGenerator.h"

int main() {
  int length;
  std::cout << "Enter password length (8 or more): ";
  std::cin >> length;

  if (length < 8) {
    std::cout << "Invalid password length. Must be 8 or more." << std::endl;
    return 1;
  }

  PasswordGenerator password_generator;

  bool use_lowercase = password_generator.ask("Use lowercase letters (y/n)? ");
  bool use_uppercase = password_generator.ask("Use uppercase letters (y/n)? ");
  bool use_numbers = password_generator.ask("Use numbers (y/n)? ");
  bool use_symbols = password_generator.ask("Use symbols (y/n)? ");

  if (!use_uppercase && !use_lowercase && !use_numbers && !use_symbols) {
    std::cout
        << "Invalid character set. At least one character set must be used."
        << std::endl;
    return 1;
  }

  // パスワード生成
  std::string password = password_generator.generate(
      length, use_lowercase, use_uppercase, use_numbers, use_symbols);

  std::cout << "Generated password: " << password << std::endl;

  return 0;
}
