#ifndef PASSWORD_GENERATOR
#define PASSWORD_GENERATOR

#include <random>
#include <string>

#include "StringUtils.h"

class PasswordGenerator {
 private:
  /* data */

 public:
  PasswordGenerator(/* args */);
  ~PasswordGenerator();

  std::string generate(int length, bool use_lowercase, bool use_uppercase,
                       bool use_number, bool use_symbol);

  bool ask(const string message);
};

PasswordGenerator::PasswordGenerator(/* args */) {}

PasswordGenerator::~PasswordGenerator() {}

std::string PasswordGenerator::generate(int length, bool use_lowercase,
                                        bool use_uppercase, bool use_number,
                                        bool use_symbol) {
  const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
  const std::string uppercase = StringUtils::toupper(lowercase);
  const std::string numbers = "0123456789";
  const std::string symbols = "!@#$%^&*()_+-=";

  std::string charaset;

  if (use_lowercase) {
    charaset += lowercase;
  }
  if (use_uppercase) {
    charaset += uppercase;
  }
  if (use_number) {
    charaset += numbers;
  }
  if (use_symbol) {
    charaset += symbols;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, charaset.size() - 1);

  std::string password;

  for (int i = 0; i < length; ++i) {
    password += charaset[dis(gen)];
  }

  return password;
};

bool PasswordGenerator::ask(const string message) {
  char input;
  std::cout << message << ": ";

  std::cin >> input;

  if (input != 'y' && input != 'n') {
    std::cout << "Invalid Answer." << std::endl;
    exit(1);
  }

  return input == 'y';
};

#endif