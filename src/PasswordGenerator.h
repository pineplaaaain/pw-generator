#ifndef PASSWORD_GENERATOR
#define PASSWORD_GENERATOR

#include <random>
#include <string>

#include "PasswordCharset.h"
#include "StringUtils.h"

class PasswordGenerator {
 private:
  /* data */
  const std::string m_lowercase = "abcdefghijklmnopqrstuvwxyz";
  const std::string m_uppercase = StringUtils::toupper(m_lowercase);
  const std::string m_numbers = "0123456789";
  const std::string m_symbols = "!@#$%^&*()_+-=";

 public:
  PasswordGenerator(/* args */);
  ~PasswordGenerator();

  std::string generate(int length, bool use_lowercase, bool use_uppercase,
                       bool use_number, bool use_symbol);

  bool ask(const PasswordCharset&);
};

PasswordGenerator::PasswordGenerator(/* args */) {}

PasswordGenerator::~PasswordGenerator() {}

std::string PasswordGenerator::generate(int length, bool use_lowercase,
                                        bool use_uppercase, bool use_number,
                                        bool use_symbol) {
  std::string charset;

  if (use_lowercase) {
    charset += m_lowercase;
  }
  if (use_uppercase) {
    charset += m_uppercase;
  }
  if (use_number) {
    charset += m_numbers;
  }
  if (use_symbol) {
    charset += m_symbols;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, charset.size() - 1);

  std::string password;

  for (int i = 0; i < length; ++i) {
    password += charset[dis(gen)];
  }

  return password;
};

bool PasswordGenerator::ask(const PasswordCharset& charset) {
  char input;
  std::cout << charset.message << "(" << charset.str << ")"
            << "(y/n)?: ";
  std::cin >> input;

  if (input != 'y' && input != 'n') {
    std::cout << "Invalid Answer." << std::endl;
    exit(1);
  }

  return input == 'y';
};

#endif