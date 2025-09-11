#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H

#include <complex>
#include <iostream>

struct DataStruct {
  unsigned long long key1;
  std::complex<double> key2;
  std::string key3;
};

struct CharIO {
  char exp;
};

struct UnsignedLongLongIO {
  unsigned long long &ref;
};

struct ComplexIO {
  std::complex<double> &ref;
};

struct StringIO {
  std::string &ref;
};

class iofmtguard {
public:
  iofmtguard(std::basic_ios<char> &s);
  ~iofmtguard();

private:
  std::basic_ios<char> &s_;
  std::streamsize width_;
  char fill_;
  std::streamsize precision_;
  std::basic_ios<char>::fmtflags fmt_;
};

std::istream &operator>>(std::istream &istream, CharIO &&dest);
std::istream &operator>>(std::istream &istream, UnsignedLongLongIO &&dest);
std::istream &operator>>(std::istream &istream, ComplexIO &&dest);
std::istream &operator>>(std::istream &istream, StringIO &&dest);
std::istream &operator>>(std::istream &istream, DataStruct &dest);
std::ostream &operator<<(std::ostream &ostream, const DataStruct &dest);

#endif
