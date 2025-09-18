#ifndef _IO_H
#define _IO_H

#include <iostream>

struct CharIO {
  char expected;
};

std::istream &operator>>(std::istream &in, CharIO &&chr);

#endif
