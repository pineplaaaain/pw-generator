#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Clipboard.h"
#include "Config.h"
#include "Entropy.h"
#include "PasswordCharset.h"
#include "PasswordGenerator.h"
#include "StringUtils.h"

// Run the interactive password generation flow.
// Returns exit code (0 = success, 1 = error).
inline int runInteractiveMode() {
  size_t length;
  std::cout << "Enter password length (" << defaults::MIN_LENGTH
            << " or more): ";

  if (!(std::cin >> length)) {
    std::cerr << "Invalid input. Please enter a number." << std::endl;
    return 1;
  }

  if (length < defaults::MIN_LENGTH) {
    std::cerr << "Invalid password length. Must be " << defaults::MIN_LENGTH
              << " or more." << std::endl;
    return 1;
  }

  const PasswordCharset lowercase{"Use lowercase letters",
                                  "abcdefghijklmnopqrstuvwxyz"};
  const PasswordCharset uppercase{"Use UPPERCASE letters",
                                  StringUtils::toupper(lowercase.str)};
  const PasswordCharset numbers{"Use numbers", "0123456789"};
  const PasswordCharset symbols{"Use symbols", "!#$%^&*()_+-="};

  bool use_lowercase = PasswordGenerator::ask(lowercase);
  bool use_uppercase = PasswordGenerator::ask(uppercase);
  bool use_numbers = PasswordGenerator::ask(numbers);
  bool use_symbols = PasswordGenerator::ask(symbols);

  if (!use_uppercase && !use_lowercase && !use_numbers && !use_symbols) {
    std::cerr
        << "Invalid character set. At least one character set must be used."
        << std::endl;
    return 1;
  }

  // Ask for count
  size_t count = defaults::DEFAULT_COUNT;
  std::cout << "How many passwords to generate? (default: 1): ";
  std::string count_input;
  std::cin.ignore();
  std::getline(std::cin, count_input);
  if (!count_input.empty()) {
    try {
      int c = std::stoi(count_input);
      if (c < 1 || static_cast<size_t>(c) > defaults::MAX_COUNT) {
        std::cerr << "Count must be between 1 and " << defaults::MAX_COUNT
                  << ". Using default (1)." << std::endl;
        count = 1;
      } else {
        count = static_cast<size_t>(c);
      }
    } catch (...) {
      count = 1;
    }
  }

  // Build charsets vector for guaranteed generation
  std::vector<std::string> charsets;
  if (use_lowercase) charsets.push_back(lowercase.str);
  if (use_uppercase) charsets.push_back(uppercase.str);
  if (use_numbers) charsets.push_back(numbers.str);
  if (use_symbols) charsets.push_back(symbols.str);

  // Generate passwords
  std::vector<std::string> passwords;
  for (size_t i = 0; i < count; ++i) {
    std::string password =
        PasswordGenerator::generateGuaranteed(length, charsets);
    if (password.empty()) {
      std::cerr << "Failed to generate password." << std::endl;
      return 1;
    }
    passwords.push_back(password);
  }

  // Output
  for (const auto& pw : passwords) {
    std::cout << pw << std::endl;
  }

  // Show entropy
  size_t charsetSize = 0;
  for (const auto& cs : charsets) {
    charsetSize += cs.size();
  }
  std::cerr << "Strength: " << Entropy::format(charsetSize, length)
            << std::endl;

  // Clipboard: auto-copy if single password
  if (count == 1) {
    if (Clipboard::copy(passwords[0])) {
      std::cout << "(Copied to clipboard)" << std::endl;
    }
  }

  return 0;
}
