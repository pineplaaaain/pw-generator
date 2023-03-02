#ifndef PASSWORD_GENERATOR
#define PASSWORD_GENERATOR

#include <random>
#include <string>

#include "PasswordCharset.h"
#include "StringUtils.h"

class PasswordGenerator {
 private:
  /* data */
  // const int m_num_passwords;
  const PasswordCharset& m_lowercase;
  const PasswordCharset& m_uppercase;
  const PasswordCharset& m_numbers;
  const PasswordCharset& m_symbols;

 public:
  PasswordGenerator(PasswordCharset& lowercase, PasswordCharset& uppercase,
                    PasswordCharset& numbers, PasswordCharset& symbols);
  ~PasswordGenerator();

  void generate(int length);
};

PasswordGenerator::PasswordGenerator(PasswordCharset& lowercase,
                                     PasswordCharset& uppercase,
                                     PasswordCharset& numbers,
                                     PasswordCharset& symbols)
    : m_lowercase(lowercase),
      m_uppercase(uppercase),
      m_numbers(numbers),
      m_symbols(symbols) {}

PasswordGenerator::~PasswordGenerator() {}

void PasswordGenerator::generate(int length) {
  std::string charset;

  if (m_lowercase.is_use) {
    charset += m_lowercase.str;
  }
  if (m_uppercase.is_use) {
    charset += m_uppercase.str;
  }
  if (m_numbers.is_use) {
    charset += m_numbers.str;
  }
  if (m_symbols.is_use) {
    charset += m_symbols.str;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, charset.size() - 1);

  std::string password;

  for (int i = 0; i < length; ++i) {
    password += charset[dis(gen)];
  }

  std::cout << "Generated password: " << password << std::endl;
};

#endif