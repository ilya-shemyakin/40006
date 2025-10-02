#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "DataStruct.h"

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> dest.ref >> DelimiterIO{ 'd' };
}

std::istream& operator>>(std::istream& in, UnsignedIO&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string str;
  char c;

  while (in.get(c)) {
    if (c == ':' || isspace(c)) {
      in.unget();
      break;
    }
    str.push_back(c);
  }

  bool has_ull_suffix = (str.size() >= 3) &&
    (tolower(str[str.size() - 1]) == 'l') &&
    (tolower(str[str.size() - 2]) == 'l') &&
    (tolower(str[str.size() - 3]) == 'u');

  try {
    if (!has_ull_suffix) {
      in.setstate(std::ios::failbit);
      return in;
    }
    std::string number = str.substr(0, str.size() - 3);
    dest.ref = std::stoull(number);
  }
  catch (...) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
  DataStruct temp;
  in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

  std::string label;
  bool flag1 = false;
  bool flag2 = false;
  bool flag3 = false;

  while (in >> label) {
    if (label == "key1") {
      in >> DoubleIO{ temp.key1 } >> DelimiterIO{ ':' };
      flag1 = true;
    }
    else if (label == "key2") {
      in >> UnsignedIO{ temp.key2 } >> DelimiterIO{ ':' };
      flag2 = true;
    }
    else if (label == "key3") {
      in >> StringIO{ temp.key3 } >> DelimiterIO{ ':' };
      flag3 = true;
    }
    else if (label == ")") {
      break;
    }
    else {
      in.setstate(std::ios::failbit);
      break;
    }
  }

  if (in && flag1 && flag2 && flag3) {
    dest = temp;
  }
  else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  iofmtguard fmtguard(out);
  out << "(:key1 " << std::fixed << std::setprecision(1) << data.key1 << "d"
    << ":key2 " << data.key2 << "ull"
    << ":key3 \"" << data.key3 << "\":)";
  return out;
}

iofmtguard::iofmtguard(std::basic_ios< char >& s) :
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

iofmtguard::~iofmtguard() {
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}