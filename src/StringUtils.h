#ifndef STRING_UTILS
#define STRING_UTILS

#include <algorithm>
#include <string>

using namespace std;

class StringUtils {
 private:
  /* data */
 public:
  StringUtils(/* args */);
  ~StringUtils();
  static string toupper(const string &str);
  static string tolower(const string &str);
};

StringUtils::StringUtils(/* args */) {}

StringUtils::~StringUtils() {}

string StringUtils::toupper(const string &str) {
  string upper = str;

  transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

  return upper;
};

string StringUtils::tolower(const string &str) {
  string lower = str;

  transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  return lower;
}

#endif