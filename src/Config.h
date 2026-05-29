#pragma once

#include <cstddef>

namespace defaults {
constexpr size_t MIN_LENGTH = 8;
constexpr size_t MAX_COUNT = 100;
constexpr size_t DEFAULT_COUNT = 1;
}  // namespace defaults

struct Config {
  size_t length = 0;
  size_t count = defaults::DEFAULT_COUNT;
  bool useLowercase = false;
  bool useUppercase = false;
  bool useNumbers = false;
  bool useSymbols = false;
  bool copyToClipboard = false;
  bool showHelp = false;
  bool isInteractive = false;

  // Returns true if no charset flags were explicitly set
  bool noCharsetSelected() const {
    return !useLowercase && !useUppercase && !useNumbers && !useSymbols;
  }

  // Enable all charsets (used when no flags specified in one-shot mode)
  void enableAllCharsets() {
    useLowercase = useUppercase = useNumbers = useSymbols = true;
  }

  // Count of selected charset categories
  size_t selectedCharsetCount() const {
    return (useLowercase ? 1 : 0) + (useUppercase ? 1 : 0) +
           (useNumbers ? 1 : 0) + (useSymbols ? 1 : 0);
  }
};
