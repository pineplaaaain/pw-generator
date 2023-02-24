#include <iostream>
#include <random>
#include <string>

#include "PasswordGenerator.h"

// // パスワード生成関数
// std::string generate_password(int length, bool use_uppercase,
//                               bool use_lowercase, bool use_numbers,
//                               bool use_symbols) {
//   const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//   const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
//   const std::string numbers = "0123456789";
//   const std::string symbols = "!@#$%^&*()_+-=";

//   // 利用する文字種別のリストを作成
//   std::string charset;
//   if (use_uppercase) {
//     charset += uppercase;
//   }
//   if (use_lowercase) {
//     charset += lowercase;
//   }
//   if (use_numbers) {
//     charset += numbers;
//   }
//   if (use_symbols) {
//     charset += symbols;
//   }

//   // パスワード生成用の乱数エンジンを作成
//   std::random_device rd;
//   std::mt19937 gen(rd());
//   std::uniform_int_distribution<> dis(0, charset.size() - 1);

//   // パスワード生成
//   std::string password;
//   for (int i = 0; i < length; ++i) {
//     password += charset[dis(gen)];
//   }

//   return password;
// }

int main() {
  int length;
  std::cout << "Enter password length (8 or more): ";
  std::cin >> length;

  if (length < 8) {
    std::cout << "Invalid password length. Must be 8 or more." << std::endl;
    return 1;
  }

  PasswordGenerator password_generator;

  bool use_lowercase = password_generator.ask("Use uppercase letters (y/n)? ");
  bool use_uppercase = password_generator.ask("Use lowercase letters (y/n)? ");
  bool use_numbers = password_generator.ask("Use numbers (y/n)? ");
  bool use_symbols = password_generator.ask("Use symbols (y/n)? ");

  if (!use_uppercase && !use_lowercase && !use_numbers && !use_symbols) {
    std::cout
        << "Invalid character set. At least one character set must be used."
        << std::endl;
    return 1;
  }

  // パスワード生成
  std::string password = password_generator.generate(
      length, use_uppercase, use_lowercase, use_numbers, use_symbols);

  std::cout << "Generated password: " << password << std::endl;

  return 0;
}
