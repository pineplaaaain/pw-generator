#include <iostream>
#include <string>

#include "PasswordCharset.h"
#include "PasswordGenerator.h"
#include "StringUtils.h"

int main() {
  size_t length;
  std::cout << "Enter password length (8 or more): ";
  
  if (!(std::cin >> length)) {
    std::cout << "Invalid input. Please enter a number." << std::endl;
    return 1;
  }

  if (length < 8) {
    std::cout << "Invalid password length. Must be 8 or more." << std::endl;
    return 1;
  }

  const PasswordCharset lowercase{"Use lowercase letters",
                                   "abcdefghijklmnopqrstuvwxyz"};
  const PasswordCharset uppercase{"Use UPPERCASE letters",
                                   StringUtils::toupper(lowercase.str)};
  const PasswordCharset numbers{"Use numbers", "0123456789"};
  const PasswordCharset symbols{"Use symbols", "!@#$%^&*()_+-="};

  bool use_lowercase = PasswordGenerator::ask(lowercase);
  bool use_uppercase = PasswordGenerator::ask(uppercase);
  bool use_numbers = PasswordGenerator::ask(numbers);
  bool use_symbols = PasswordGenerator::ask(symbols);

  if (!use_uppercase && !use_lowercase && !use_numbers && !use_symbols) {
    std::cout << "Invalid character set. At least one character set must be used."
              << std::endl;
    return 1;
  }

  std::string charset;
  if (use_lowercase) charset += lowercase.str;
  if (use_uppercase) charset += uppercase.str;
  if (use_numbers) charset += numbers.str;
  if (use_symbols) charset += symbols.str;

  std::string password = PasswordGenerator::generate(length, charset);

  if (password.empty()) {
    std::cout << "Failed to generate password." << std::endl;
    return 1;
  }

  std::cout << "Generated password: " << password << std::endl;

  return 0;
}
