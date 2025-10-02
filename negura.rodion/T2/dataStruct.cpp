#include "DataStruct.h"
#include <sstream>
#include <cctype>
#include <iomanip>
#include <algorithm>

namespace nspace {

  static bool parseDoubleWithSuffix(const std::string& token, double& out) {
    size_t len = token.size();
    if (len < 2) return false;
    if (token[len - 1] != 'd' && token[len - 1] != 'D') return false;
    std::string numberPart = token.substr(0, len - 1);
    try {
      size_t pos = 0;
      out = std::stod(numberPart, &pos);
      if (pos != numberPart.size()) return false;
    }
    catch (...) {
      return false;
    }
    return true;
  }

  static bool parseUnsignedLongLongWithSuffix(const std::string& token, unsigned long long& out) {
    std::string s = token;
    if (s.size() < 3) return false;
    std::string suffix = s.substr(s.size() - 3);
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
    if (suffix != "ull") return false;
    std::string numberPart = s.substr(0, s.size() - 3);
    try {
      size_t pos = 0;
      out = std::stoull(numberPart, &pos, 0);
      if (pos != numberPart.size()) return false;
    }
    catch (...) {
      return false;
    }
    return true;
  }

  static bool parseStringWithQuotes(const std::string& token, std::string& out) {
    if (token.size() < 2) return false;
    if (token.front() != '"' || token.back() != '"') return false;
    out = token.substr(1, token.size() - 2);
    return true;
  }

  std::istream& operator>>(std::istream& input, DataStruct& destination) {
    std::string line;
    if (!std::getline(input, line)) {
      input.setstate(std::ios::failbit);
      return input;
    }

    size_t start = line.find("(:");
    size_t end = line.rfind(":)");
    if (start == std::string::npos || end == std::string::npos || start >= end) {
      input.setstate(std::ios::failbit);
      return input;
    }

    std::string content = line.substr(start + 2, end - start - 2);

    destination.key1 = 0;
    destination.key2 = 0;
    destination.key3.clear();

    std::istringstream ss(content);
    std::string token;

    bool key1_found = false;
    bool key2_found = false;
    bool key3_found = false;

    while (std::getline(ss, token, ':')) {
      if (token.empty()) continue;
      size_t space_pos = token.find(' ');
      if (space_pos == std::string::npos) {
        input.setstate(std::ios::failbit);
        return input;
      }
      std::string key = token.substr(0, space_pos);
      std::string value = token.substr(space_pos + 1);

      if (key == "key1") {
        if (key1_found || !parseDoubleWithSuffix(value, destination.key1)) {
          input.setstate(std::ios::failbit);
          return input;
        }
        key1_found = true;
      }
      else if (key == "key2") {
        if (key2_found || !parseUnsignedLongLongWithSuffix(value, destination.key2)) {
          input.setstate(std::ios::failbit);
          return input;
        }
        key2_found = true;
      }
      else if (key == "key3") {
        if (key3_found || !parseStringWithQuotes(value, destination.key3)) {
          input.setstate(std::ios::failbit);
          return input;
        }
        key3_found = true;
      }
      else {
        input.setstate(std::ios::failbit);
        return input;
      }
    }

    if (!(key1_found && key2_found && key3_found)) {
      input.setstate(std::ios::failbit);
      return input;
    }

    return input;
  }

  std::ostream& operator<<(std::ostream& output, const DataStruct& source) {
    output << "(:key1 " << std::fixed << std::setprecision(1) << source.key1 << "d:"
           << "key2 " << source.key2 << "ull:"
           << "key3 \"" << source.key3 << "\":)";
    return output;
  }

  bool compareDataStructs(const DataStruct& first, const DataStruct& second) {
    if (first.key1 != second.key1) return first.key1 < second.key1;
    if (first.key2 != second.key2) return first.key2 < second.key2;
    return first.key3.length() < second.key3.length();
  }

}
