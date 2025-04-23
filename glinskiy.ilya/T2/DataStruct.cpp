#include "DataStruct.h"
#include <cassert>
#include <complex>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::istream &operator>>(std::istream &istream, DelimiterIO &&dest) {
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

  std::string str;
  char chr;

  while (istream.get(chr)) {
    if (chr == ':') {
      istream.unget();
      break;
    }

    str.push_back(tolower(chr));
  }

  if (str.size() > 2 && str.substr(0, 2) == "0x") {
    try {
      dest.ref = std::stoull(str, nullptr, 16);
    } catch (...) {
      istream.setstate(std::ios::failbit);
    };
  } else
    istream.setstate(std::ios::failbit);

  return istream;
}

std::istream &operator>>(std::istream &istream, ComplexIO &&dest) {
  std::istream::sentry sentry(istream);
  if (!sentry)
    return istream;

  std::string str, real, imag;
  char chr;

  while (istream.get(chr)) {
    if (chr == ':') {
      istream.unget();
      break;
    }

    str.push_back(tolower(chr));
  }

  bool is_enclosed = str.substr(0, 3) == "#c(" && str[str.size() - 1] == ')';
  int space_pos = str.find(' ', 3);

  if (str.size() > 6 && is_enclosed &&
      static_cast<std::size_t>(space_pos) != std::string::npos)
    try {
      double real = std::stod(str.substr(3, space_pos));
      double imag = std::stod(str.substr(space_pos + 1, str.size() - 1));
      dest.ref = std::complex<double>(real, imag);
    } catch (...) {
      istream.setstate(std::ios::failbit);
    }
  else
    istream.setstate(std::ios::failbit);

  return istream;
}

std::istream &operator>>(std::istream &istream, StringIO &&dest) {
  std::istream::sentry sentry(istream);
  if (!sentry)
    return istream;

  return std::getline(istream >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream &operator>>(std::istream &istream, DataStruct &dest) {
  std::istream::sentry sentry(istream);
  if (!sentry)
    return istream;

  DataStruct input;
  std::string str;
  bool has1 = false, has2 = false, has3 = false;

  istream >> DelimiterIO{'('} >> DelimiterIO{':'};
  while (istream >> str) {
    if (str == "key1") {
      istream >> UnsignedLongLongIO{input.key1} >> DelimiterIO{':'};
      if (istream)
        has1 = true;

    } else if (str == "key2") {
      istream >> ComplexIO{input.key2} >> DelimiterIO{':'};
      if (istream)
        has2 = true;

    } else if (str == "key3") {
      istream >> StringIO{input.key3} >> DelimiterIO{':'};
      if (istream)
        has3 = true;

    } else if (str == ")") {
      if (!has1 || !has2 || !has3)
        istream.setstate(std::ios::failbit);
      break;

    } else {
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

  ostream << "(:key1 0x" << std::hex << std::uppercase << src.key1
          << ":key2 #c(" << src.key2.real() << " " << src.key2.imag() << ")"
          << ":key3 \"" << src.key3 << "\":)";

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
