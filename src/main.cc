#include <iostream>
#include <string>
#include <vector>

#include "ArgParser.h"
#include "Clipboard.h"
#include "Config.h"
#include "Entropy.h"
#include "InteractiveMode.h"
#include "PasswordCharset.h"
#include "PasswordGenerator.h"
#include "StringUtils.h"

int main(int argc, char* argv[]) {
  // Interactive mode: no arguments
  if (argc == 1) {
    return runInteractiveMode();
  }

  // One-shot mode: parse arguments
  Config config;
  try {
    config = ArgParser::parse(argc, argv);
  } catch (const ArgParser::ParseError& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "Use --help for usage information." << std::endl;
    return 1;
  }

  if (config.showHelp) {
    ArgParser::printHelp(argv[0]);
    return 0;
  }

  // Build charsets
  const std::string lowercase_str = "abcdefghijklmnopqrstuvwxyz";
  const std::string uppercase_str = StringUtils::toupper(lowercase_str);
  const std::string numbers_str = "0123456789";
  const std::string symbols_str = "!#$%^&*()_+-=";

  std::vector<std::string> charsets;
  if (config.useLowercase) charsets.push_back(lowercase_str);
  if (config.useUppercase) charsets.push_back(uppercase_str);
  if (config.useNumbers) charsets.push_back(numbers_str);
  if (config.useSymbols) charsets.push_back(symbols_str);

  // Validate length against charset count
  if (config.length < charsets.size()) {
    std::cerr << "Error: Password length must be at least "
              << charsets.size()
              << " to include all selected character types." << std::endl;
    return 1;
  }

  // Generate passwords
  std::vector<std::string> passwords;
  for (size_t i = 0; i < config.count; ++i) {
    std::string password =
        PasswordGenerator::generateGuaranteed(config.length, charsets);
    if (password.empty()) {
      std::cerr << "Error: Failed to generate password." << std::endl;
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
  std::cerr << "Strength: " << Entropy::format(charsetSize, config.length)
            << std::endl;

  // Clipboard handling
  bool shouldCopy =
      config.copyToClipboard || (config.count == 1);

  if (shouldCopy) {
    std::string clipContent;
    for (size_t i = 0; i < passwords.size(); ++i) {
      if (i > 0) clipContent += "\n";
      clipContent += passwords[i];
    }
    if (Clipboard::copy(clipContent)) {
      std::cerr << "(Copied to clipboard)" << std::endl;
    }
  }

  return 0;
}
