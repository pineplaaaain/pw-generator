#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "../src/ArgParser.h"
#include "../src/Clipboard.h"
#include "../src/Config.h"
#include "../src/PasswordGenerator.h"

// ============================================================
// Lightweight test framework
// ============================================================

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name)                                              \
  static void test_##name();                                   \
  static struct Register_##name {                              \
    Register_##name() { test_registry().push_back({#name, test_##name}); } \
  } register_##name;                                           \
  static void test_##name()

#define ASSERT_TRUE(expr)                                          \
  do {                                                             \
    if (!(expr)) {                                                 \
      std::cerr << "  FAIL: " << #expr << " (line " << __LINE__   \
                << ")" << std::endl;                               \
      throw std::runtime_error("assertion failed");                \
    }                                                              \
  } while (0)

#define ASSERT_EQ(a, b)                                            \
  do {                                                             \
    if ((a) != (b)) {                                              \
      std::cerr << "  FAIL: " << #a << " == " << #b << " (line "  \
                << __LINE__ << ")" << std::endl;                   \
      std::cerr << "    got: " << (a) << " vs " << (b)            \
                << std::endl;                                      \
      throw std::runtime_error("assertion failed");                \
    }                                                              \
  } while (0)

#define ASSERT_THROWS(expr, exception_type)                        \
  do {                                                             \
    bool caught = false;                                           \
    try { expr; } catch (const exception_type&) { caught = true; } \
    if (!caught) {                                                 \
      std::cerr << "  FAIL: expected " << #exception_type          \
                << " from " << #expr << " (line " << __LINE__      \
                << ")" << std::endl;                               \
      throw std::runtime_error("assertion failed");                \
    }                                                              \
  } while (0)

struct TestEntry {
  const char* name;
  void (*func)();
};

static std::vector<TestEntry>& test_registry() {
  static std::vector<TestEntry> registry;
  return registry;
}

// ============================================================
// Helper: create argv from strings
// ============================================================

static Config parseArgs(std::initializer_list<const char*> args) {
  std::vector<char*> argv;
  for (auto a : args) {
    argv.push_back(const_cast<char*>(a));
  }
  return ArgParser::parse(static_cast<int>(argv.size()), argv.data());
}

// ============================================================
// Tests: ArgParser
// ============================================================

TEST(help_flag_sets_showHelp) {
  Config c = parseArgs({"pwgen", "--help"});
  ASSERT_TRUE(c.showHelp);
}

TEST(short_help_flag) {
  Config c = parseArgs({"pwgen", "-h"});
  ASSERT_TRUE(c.showHelp);
}

TEST(copy_flag_sets_copyToClipboard) {
  Config c = parseArgs({"pwgen", "16", "--copy"});
  ASSERT_TRUE(c.copyToClipboard);
}

TEST(no_args_sets_interactive) {
  Config c = parseArgs({"pwgen"});
  ASSERT_TRUE(c.isInteractive);
}

TEST(combined_flags_lns) {
  Config c = parseArgs({"pwgen", "16", "-lns"});
  ASSERT_TRUE(c.useLowercase);
  ASSERT_TRUE(!c.useUppercase);
  ASSERT_TRUE(c.useNumbers);
  ASSERT_TRUE(c.useSymbols);
}

TEST(all_charsets_when_none_specified) {
  Config c = parseArgs({"pwgen", "16"});
  ASSERT_TRUE(c.useLowercase);
  ASSERT_TRUE(c.useUppercase);
  ASSERT_TRUE(c.useNumbers);
  ASSERT_TRUE(c.useSymbols);
}

TEST(count_option) {
  Config c = parseArgs({"pwgen", "16", "-c", "5"});
  ASSERT_EQ(c.count, static_cast<size_t>(5));
}

TEST(count_too_large_throws) {
  ASSERT_THROWS(parseArgs({"pwgen", "16", "-c", "101"}),
                ArgParser::ParseError);
}

TEST(count_zero_throws) {
  ASSERT_THROWS(parseArgs({"pwgen", "16", "-c", "0"}),
                ArgParser::ParseError);
}

TEST(length_too_short_throws) {
  ASSERT_THROWS(parseArgs({"pwgen", "3"}), ArgParser::ParseError);
}

TEST(invalid_length_throws) {
  ASSERT_THROWS(parseArgs({"pwgen", "abc"}), ArgParser::ParseError);
}

TEST(unknown_flag_throws) {
  ASSERT_THROWS(parseArgs({"pwgen", "16", "-x"}), ArgParser::ParseError);
}

// ============================================================
// Tests: Config helpers
// ============================================================

TEST(config_noCharsetSelected) {
  Config c;
  ASSERT_TRUE(c.noCharsetSelected());
  c.useLowercase = true;
  ASSERT_TRUE(!c.noCharsetSelected());
}

TEST(config_selectedCharsetCount) {
  Config c;
  c.useLowercase = true;
  c.useNumbers = true;
  ASSERT_EQ(c.selectedCharsetCount(), static_cast<size_t>(2));
}

TEST(config_enableAllCharsets) {
  Config c;
  c.enableAllCharsets();
  ASSERT_EQ(c.selectedCharsetCount(), static_cast<size_t>(4));
}

// ============================================================
// Tests: PasswordGenerator::generateGuaranteed
// ============================================================

TEST(generateGuaranteed_contains_all_charsets) {
  std::vector<std::string> charsets = {"abc", "123", "!@#"};
  // Run multiple times to reduce flakiness
  for (int i = 0; i < 50; ++i) {
    std::string pw = PasswordGenerator::generateGuaranteed(10, charsets);
    ASSERT_EQ(pw.size(), static_cast<size_t>(10));
    // Must contain at least one from each charset
    bool has_alpha = pw.find_first_of("abc") != std::string::npos;
    bool has_digit = pw.find_first_of("123") != std::string::npos;
    bool has_symbol = pw.find_first_of("!@#") != std::string::npos;
    ASSERT_TRUE(has_alpha);
    ASSERT_TRUE(has_digit);
    ASSERT_TRUE(has_symbol);
  }
}

TEST(generateGuaranteed_returns_empty_if_length_too_short) {
  std::vector<std::string> charsets = {"abc", "123", "!@#"};
  std::string pw = PasswordGenerator::generateGuaranteed(2, charsets);
  ASSERT_TRUE(pw.empty());
}

TEST(generateGuaranteed_empty_charsets_returns_empty) {
  std::vector<std::string> charsets = {};
  std::string pw = PasswordGenerator::generateGuaranteed(10, charsets);
  ASSERT_TRUE(pw.empty());
}

// ============================================================
// Tests: Clipboard auto-copy logic
// ============================================================

TEST(clipboard_auto_copy_when_count_1) {
  // Simulating the logic in main.cc
  Config c;
  c.count = 1;
  c.copyToClipboard = false;
  bool shouldCopy = c.copyToClipboard || (c.count == 1);
  ASSERT_TRUE(shouldCopy);
}

TEST(clipboard_skipped_when_count_gt_1_without_copy) {
  Config c;
  c.count = 5;
  c.copyToClipboard = false;
  bool shouldCopy = c.copyToClipboard || (c.count == 1);
  ASSERT_TRUE(!shouldCopy);
}

TEST(clipboard_copies_when_count_gt_1_with_copy_flag) {
  Config c;
  c.count = 5;
  c.copyToClipboard = true;
  bool shouldCopy = c.copyToClipboard || (c.count == 1);
  ASSERT_TRUE(shouldCopy);
}

// ============================================================
// Main: run all tests
// ============================================================

int main() {
  std::cout << "Running tests..." << std::endl;
  std::cout << std::string(50, '=') << std::endl;

  for (const auto& test : test_registry()) {
    ++tests_run;
    try {
      test.func();
      ++tests_passed;
      std::cout << "  PASS: " << test.name << std::endl;
    } catch (...) {
      ++tests_failed;
    }
  }

  std::cout << std::string(50, '=') << std::endl;
  std::cout << "Results: " << tests_passed << "/" << tests_run << " passed";
  if (tests_failed > 0) {
    std::cout << ", " << tests_failed << " FAILED";
  }
  std::cout << std::endl;

  return tests_failed > 0 ? 1 : 0;
}
