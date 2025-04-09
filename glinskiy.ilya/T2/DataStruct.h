#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H

#include <complex>
#include <iostream>
#include <string>

struct DataStruct {
  unsigned long long key1;
  std::complex<double> key2;
  std::string key3;

  friend std::ostream &operator<<(std::ostream &ostream, DataStruct struc) {
    ostream << "(:key1 " << struc.key1 << ":key2 " << abs(struc.key2)
            << ":key3 " << struc.key3 << ":)";

    return ostream;
  }
};

#endif