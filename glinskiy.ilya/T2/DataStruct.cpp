#include "DataStruct.h"
#include <cassert>
#include <complex>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::istream &operator>>(std::istream &istream, CharIO &&dest) {
  std::istream::sentry sentry(istream);
  if (!sentry)
    return istream;

  char c = '0';
  istream >> c;

  if (istream && (c != dest.exp))
    istream.setstate(std::ios::failbit);

  return istream;
}

std::istream &operator>>(std::istream &istream, UnsignedLongLongIO &&dest) {
  std::istream::sentry sentry(istream);
  if (!sentry)
    return istream;

  istream >> CharIO{'0'} >> CharIO{'x'} >> std::hex >> dest.ref;

  return istream;
}

std::istream &operator>>(std::istream &istream, ComplexIO &&dest) {
  std::istream::sentry sentry(istream);
  if (!sentry)
    return istream;

  double real, imag;

  istream >> CharIO{'#'} >> CharIO{'c'} >> CharIO{'('} >> real >> imag >>
      CharIO{')'};

  dest.ref = std::complex<double>(real, imag);

  return istream;
}

std::istream &operator>>(std::istream &istream, StringIO &&dest) {
  std::istream::sentry sentry(istream);
  if (!sentry)
    return istream;

  return std::getline(istream >> CharIO{'"'}, dest.ref, '"');
}

std::istream &operator>>(std::istream &istream, DataStruct &dest) {
  std::istream::sentry sentry(istream);
  if (!sentry)
    return istream;

  iofmtguard fmtguard(istream);

  DataStruct input;
  std::string str;

  istream >> CharIO{'('} >> CharIO{':'};
  while (istream >> str) {
    if (str == "key1")
      istream >> UnsignedLongLongIO{input.key1} >> CharIO{':'};
    else if (str == "key2")
      istream >> ComplexIO{input.key2} >> CharIO{':'};
    else if (str == "key3")
      istream >> StringIO{input.key3} >> CharIO{':'};
    else if (str == ")")
      break;
    else {
      istream.setstate(std::ios::failbit);
      break;
    }
  }

  if (istream)
    dest = input;

  return istream;
}

std::ostream &operator<<(std::ostream &ostream, const DataStruct &src) {
  std::ostream::sentry sentry(ostream);
  if (!sentry)
    return ostream;

  iofmtguard fmtguard(ostream);

  ostream << "(:key1 0x" << std::hex << std::uppercase << src.key1;
  ostream << ":key2 #c(" << std::fixed << std::setprecision(1)
          << src.key2.real() << " " << src.key2.imag() << ")";
  ostream << ":key3 \"" << src.key3 << "\":)";

  return ostream;
}

iofmtguard::iofmtguard(std::basic_ios<char> &s)
    : s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()),
      fmt_(s.flags()) {}

iofmtguard::~iofmtguard() {
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
