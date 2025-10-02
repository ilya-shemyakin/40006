#ifndef DATASTRUCT_H_
#define DATASTRUCT_H_
#include <iostream>

namespace nspace {
  struct DataStruct {
    double key1;           // DBL LIT
    unsigned long long key2; // ULL LIT
    std::string key3;
  };

  struct DelimiterIO {
    char exp;
  };

  struct DoubleIO {
    double& num;
  };

  struct UllLitIO {
    unsigned long long& num;
  };

  struct StringIO {
    std::string& ref;
  };

  class iofmtguard
  {
  public:
    iofmtguard(std::basic_ios<char>& s);
    ~iofmtguard();
  private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dbl);
  std::istream& operator>>(std::istream& in, UllLitIO&& lit);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  bool compare(const DataStruct& a, const DataStruct& b);
}
#endif