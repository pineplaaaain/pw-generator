#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "Config.h"

class ArgParser {
 public:
  struct ParseError : std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  // Parse command-line arguments into a Config.
  // Throws ParseError on invalid input.
  static Config parse(int argc, char* argv[]) {
    Config config;

    if (argc < 2) {
      config.isInteractive = true;
      return config;
    }

    int i = 1;

    // First positional argument: password length
    std::string first(argv[1]);
    if (first == "--help" || first == "-h") {
      config.showHelp = true;
      return config;
    }

    try {
      int len = std::stoi(first);
      if (len < 0) {
        throw ParseError("Password length must be a positive number.");
      }
      config.length = static_cast<size_t>(len);
    } catch (const std::invalid_argument&) {
      throw ParseError("Invalid password length: " + first);
    } catch (const std::out_of_range&) {
      throw ParseError("Password length out of range: " + first);
    }

    if (config.length < defaults::MIN_LENGTH) {
      throw ParseError("Password length must be " +
                       std::to_string(defaults::MIN_LENGTH) + " or more.");
    }

    i = 2;

    // Parse remaining options
    while (i < argc) {
      std::string arg(argv[i]);

      if (arg == "--copy") {
        config.copyToClipboard = true;
        ++i;
      } else if (arg == "--help" || arg == "-h") {
        config.showHelp = true;
        return config;
      } else if (arg == "-c") {
        ++i;
        if (i >= argc) {
          throw ParseError("-c requires a count argument.");
        }
        try {
          int count = std::stoi(std::string(argv[i]));
          if (count < 1 || static_cast<size_t>(count) > defaults::MAX_COUNT) {
            throw ParseError("Count must be between 1 and " +
                             std::to_string(defaults::MAX_COUNT) + ".");
          }
          config.count = static_cast<size_t>(count);
        } catch (const std::invalid_argument&) {
          throw ParseError("Invalid count: " + std::string(argv[i]));
        } catch (const std::out_of_range&) {
          throw ParseError("Count out of range: " + std::string(argv[i]));
        }
        ++i;
      } else if (arg.size() >= 2 && arg[0] == '-' && arg[1] != '-') {
        // Combined short flags: -l, -u, -n, -s (e.g. -lns)
        for (size_t j = 1; j < arg.size(); ++j) {
          switch (arg[j]) {
            case 'l':
              config.useLowercase = true;
              break;
            case 'u':
              config.useUppercase = true;
              break;
            case 'n':
              config.useNumbers = true;
              break;
            case 's':
              config.useSymbols = true;
              break;
            default:
              throw ParseError("Unknown option: -" + std::string(1, arg[j]));
          }
        }
        ++i;
      } else {
        throw ParseError("Unknown argument: " + arg);
      }
    }

    // If no charset flags given, enable all
    if (config.noCharsetSelected()) {
      config.enableAllCharsets();
    }

    return config;
  }

  // Print usage/help text to stdout.
  static void printHelp(const std::string& programName) {
    std::cout << "Usage: " << programName << " [length] [options]\n"
              << "\n"
              << "Generate secure random passwords.\n"
              << "\n"
              << "Arguments:\n"
              << "  length          Password length (8 or more)\n"
              << "\n"
              << "Options:\n"
              << "  -l              Include lowercase letters (a-z)\n"
              << "  -u              Include uppercase letters (A-Z)\n"
              << "  -n              Include numbers (0-9)\n"
              << "  -s              Include symbols (!#$%^&*()_+-=)\n"
              << "  -c <count>      Number of passwords to generate (1-"
              << defaults::MAX_COUNT << ")\n"
              << "  --copy          Copy passwords to clipboard\n"
              << "  --help, -h      Show this help message\n"
              << "\n"
              << "If no charset options are specified, all are used.\n"
              << "If no arguments are given, interactive mode is used.\n"
              << "\n"
              << "Examples:\n"
              << "  " << programName << " 16          # 16 chars, all charsets\n"
              << "  " << programName << " 20 -ln      # 20 chars, lowercase + numbers\n"
              << "  " << programName << " 16 -c 5     # Generate 5 passwords\n"
              << "  " << programName
              << " 16 -c 5 --copy  # Generate 5 and copy to clipboard\n";
  }
};
