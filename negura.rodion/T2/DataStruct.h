#ifndef DATASTRUCT_H_
#define DATASTRUCT_H_

#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <algorithm>

namespace nspace {

  struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

  std::istream& operator>>(std::istream& input, DataStruct& destination);
  std::ostream& operator<<(std::ostream& output, const DataStruct& source);

  bool compareDataStructs(const DataStruct& first, const DataStruct& second);

}

#endif
