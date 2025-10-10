#include "IO.h"

std::istream &operator>>(std::istream &in, CharIO &&chr) {
  char input;
  in >> input;

  if (input != chr.expected)
    in.setstate(std::ios::failbit);

  return in;
};
