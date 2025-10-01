#ifndef DATASTRUCT_H_
#define DATASTRUCT_H_

#include <iostream>
#include <string>

namespace nspace {

  struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  struct DelimiterIO {
    char expected;
  };

  struct DoubleLiteralIO {
    double& reference;
  };

  struct UnsignedLongLongLiteralIO {
    unsigned long long& reference;
  };

  struct StringIO {
    std::string& reference;
  };

  struct LabelIO {
    std::string expected;
  };

  class IoFormatGuard {
  public:
    IoFormatGuard(std::basic_ios<char>& stream);
    ~IoFormatGuard();
  private:
    std::basic_ios<char>& stream_;
    char fillCharacter_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags formatFlags_;
  };

  std::istream& operator>>(std::istream& input, DelimiterIO&& destination);
  std::istream& operator>>(std::istream& input, DoubleLiteralIO&& destination);
  std::istream& operator>>(std::istream& input, UnsignedLongLongLiteralIO&& destination);
  std::istream& operator>>(std::istream& input, StringIO&& destination);
  std::istream& operator>>(std::istream& input, LabelIO&& destination);
  std::istream& operator>>(std::istream& input, DataStruct& destination);
  std::ostream& operator<<(std::ostream& output, const DataStruct& source);

  bool compareDataStructs(const DataStruct& first, const DataStruct& second);

}

#endif
