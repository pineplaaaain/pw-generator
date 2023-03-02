#include <iostream>
#include <random>
#include <string>

#include "PasswordCharset.h"
#include "PasswordGenerator.h"
#include "StringUtils.h"

int main() {
  int length;
  std::cout << "Enter password length (8 or more): ";
  std::cin >> length;

  if (length < 8) {
    std::cout << "Invalid password length. Must be 8 or more." << std::endl;
    return 1;
  }

  PasswordCharset lowercase("Use lowercase letters",
                            "abcdefghijklmnopqrstuvwxyz");
  PasswordCharset uppercase("Use UPPERCASE letters",
                            StringUtils::toupper(lowercase.str));

  PasswordCharset numbers("Use numbers", "0123456789");
  PasswordCharset symbols("Use symbols", "!@#$%^&*()_+-=");

  if (!lowercase.is_use && !uppercase.is_use && !numbers.is_use &&
      !symbols.is_use) {
    std::cout
        << "Invalid character set. At least one character set must be used."
        << std::endl;
    return 1;
  }

  PasswordGenerator password_generator(lowercase, uppercase, numbers, symbols);

  // パスワード生成
  password_generator.generate(length);

  return 0;
};
