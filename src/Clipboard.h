#pragma once

#include <cstdio>
#include <iostream>
#include <string>

class Clipboard {
 public:
  // Copy text to system clipboard.
  // Returns true on success, false on failure (prints warning to stderr).
  static bool copy(const std::string& text) {
    const char* cmd = getCommand();

#ifdef _WIN32
    FILE* pipe = _popen(cmd, "w");
#else
    FILE* pipe = popen(cmd, "w");
#endif

    if (!pipe) {
      std::cerr << "Warning: Could not open clipboard (command: " << cmd
                << ")" << std::endl;
      return false;
    }

    size_t written = fwrite(text.c_str(), 1, text.size(), pipe);

#ifdef _WIN32
    int status = _pclose(pipe);
#else
    int status = pclose(pipe);
#endif

    if (status != 0 || written != text.size()) {
      std::cerr << "Warning: Failed to copy to clipboard." << std::endl;
      return false;
    }

    return true;
  }

 private:
  static const char* getCommand() {
#if defined(__APPLE__)
    return "pbcopy";
#elif defined(_WIN32)
    return "clip";
#else
    return "xclip -selection clipboard";
#endif
  }
};
