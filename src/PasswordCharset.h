#ifndef PASSWORD_CHARSET
#define PASSWORD_CHARSET

#include <string>

class PasswordCharset {
 private:
  /* data */
  const std::string m_message;

 public:
  const std::string str;
  bool is_use;

  PasswordCharset(std::string message, std::string str);
  ~PasswordCharset();
};

PasswordCharset::PasswordCharset(std::string message, std::string str)
    : is_use(false), m_message(message), str(str) {
  char input;
  std::cout << m_message << " -- " << str << " (y/n)?: ";
  std::cin >> input;

  if (input != 'y' && input != 'n') {
    std::cout << "Invalid Answer." << std::endl;
    exit(1);
  }

  is_use = input == 'y';
}

PasswordCharset::~PasswordCharset() {}

#endif