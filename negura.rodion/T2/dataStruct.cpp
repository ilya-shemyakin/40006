#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <limits>
#include <iomanip>
#include "DataStruct.h"

namespace nspace {

  const char DOUBLE_SUFFIX = 'd';
  const char QUOTE_CHAR = '"';
  const char OPEN_BRACKET = '(';
  const char CLOSE_BRACKET = ')';
  const char COLON = ':';
  const std::string KEY1_LABEL = "key1";
  const std::string KEY2_LABEL = "key2";
  const std::string KEY3_LABEL = "key3";
  const std::string ULL_SUFFIX = "ull";

  std::istream& operator>>(std::istream& input, DelimiterIO&& destination) {
    std::istream::sentry sentry(input);
    if (!sentry) {
      return input;
    }

    char character = '\0';
    input >> character;
    if (input && (character != destination.expected)) {
      input.setstate(std::ios::failbit);
    }
    return input;
  }

  std::istream& operator>>(std::istream& input, DoubleLiteralIO&& destination) {
    std::istream::sentry sentry(input);
    if (!sentry) {
      return input;
    }

    double value = 0.0;
    input >> value;
    if (!input) {
      return input;
    }

    char suffix = '\0';
    input >> suffix;
    if (suffix != DOUBLE_SUFFIX && suffix != std::toupper(DOUBLE_SUFFIX)) {
      input.setstate(std::ios::failbit);
      return input;
    }

    destination.reference = value;
    return input;
  }

  std::istream& operator>>(std::istream& input, UnsignedLongLongLiteralIO&& destination) {
    std::istream::sentry sentry(input);
    if (!sentry) {
      return input;
    }

    unsigned long long value = 0;
    input >> value;
    if (!input) {
      return input;
    }


    char u, l1, l2;
    input >> u >> l1 >> l2;

    if (u != 'u' || l1 != 'l' || l2 != 'l') {
      input.setstate(std::ios::failbit);
      return input;
    }

    destination.reference = value;
    return input;
  }

  std::istream& operator>>(std::istream& input, StringIO&& destination) {
    std::istream::sentry sentry(input);
    if (!sentry) {
      return input;
    }

    input >> DelimiterIO{ QUOTE_CHAR };
    if (!input) {
      return input;
    }
    std::getline(input, destination.reference, QUOTE_CHAR);
    return input;
  }

  std::istream& operator>>(std::istream& input, LabelIO&& destination) {
    std::istream::sentry sentry(input);
    if (!sentry) {
      return input;
    }

    std::string data;
    if ((input >> data) && (data != destination.expected)) {
      input.setstate(std::ios::failbit);
    }
    return input;
  }

  std::istream& operator>>(std::istream& input, DataStruct& destination) {
    std::istream::sentry sentry(input);
    if (!sentry) {
      return input;
    }

    DataStruct inputData;
    bool isKey1Found = false;
    bool isKey2Found = false;
    bool isKey3Found = false;

    input >> DelimiterIO{ OPEN_BRACKET } >> DelimiterIO{ COLON };

    while (input) {
      if (input.peek() == CLOSE_BRACKET) {
        input.ignore();
        break;
      }

      std::string fieldName;
      if (!(input >> fieldName)) {
        break;
      }

      if (fieldName == KEY1_LABEL) {
        input >> DoubleLiteralIO{ inputData.key1 } >> DelimiterIO{ COLON };
        isKey1Found = true;
      }
      else if (fieldName == KEY2_LABEL) {
        input >> UnsignedLongLongLiteralIO{ inputData.key2 } >> DelimiterIO{ COLON };
        isKey2Found = true;
      }
      else if (fieldName == KEY3_LABEL) {
        input >> StringIO{ inputData.key3 } >> DelimiterIO{ COLON };
        isKey3Found = true;
      }
      else {
        input.setstate(std::ios::failbit);
        break;
      }
    }

    if (input && isKey1Found && isKey2Found && isKey3Found) {
      destination = inputData;
    }
    else {
      input.setstate(std::ios::failbit);
    }

    return input;
  }

  std::ostream& operator<<(std::ostream& output, const DataStruct& source) {
    std::ostream::sentry sentry(output);
    if (!sentry) {
      return output;
    }

    IoFormatGuard formatGuard(output);
    output << "(:key1 " << source.key1 << "d:key2 " << source.key2
      << "ull :key3 \"" << source.key3 << "\":)";
    return output;
  }

  IoFormatGuard::IoFormatGuard(std::basic_ios<char>& stream) :
    stream_(stream),
    fillCharacter_(stream.fill()),
    width_(stream.width()),
    precision_(stream.precision()),
    formatFlags_(stream.flags())
  {}

  IoFormatGuard::~IoFormatGuard() {
    stream_.fill(fillCharacter_);
    stream_.width(width_);
    stream_.precision(precision_);
    stream_.flags(formatFlags_);
  }

  bool compareDataStructs(const DataStruct& first, const DataStruct& second) {
    if (first.key1 != second.key1) {
      return first.key1 < second.key1;
    }
    if (first.key2 != second.key2) {
      return first.key2 < second.key2;
    }
    return first.key3.length() < second.key3.length();
  }

}
