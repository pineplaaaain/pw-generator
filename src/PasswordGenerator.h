#ifndef PASSWORD_GENERATOR
#define PASSWORD_GENERATOR

#include <openssl/rand.h>
#include <stdexcept>
#include <string>

#include "PasswordCharset.h"
#include "StringUtils.h"

class PasswordGenerator {
 private:
  /* data */
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

  std::string password;
  unsigned char random_bytes[length];

  if (RAND_bytes(random_bytes, length) != 1) {
    throw std::runtime_error("Failed to generate cryptographically secure random bytes");
  }

  for (int i = 0; i < length; ++i) {
    password += charset[random_bytes[i] % charset.size()];
  }

  std::cout << "Generated password: " << password << std::endl;
};

#endif