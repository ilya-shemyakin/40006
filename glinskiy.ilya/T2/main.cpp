#include "DataStruct.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

bool comp(DataStruct a, DataStruct b);

int main() {
  std::vector<DataStruct> vector;

  std::copy(std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(), std::back_inserter(vector));

  std::sort(vector.begin(), vector.end(), comp);

  std::copy(vector.begin(), vector.end(),
            std::ostream_iterator<DataStruct>(std::cout, "\n"));
}

bool comp(DataStruct a, DataStruct b) {
  if (a.key1 != b.key1)
    return a.key1 < b.key1;

  if (a.key2 != b.key2)
    return abs(a.key2) < abs(b.key2);

  return a.key3.length() < b.key3.length();
}
