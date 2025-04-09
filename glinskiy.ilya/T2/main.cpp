#include "DataStruct.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <string>
#include <vector>

bool comp(DataStruct a, DataStruct b) {
  if (a.key1 != b.key1)
    return a.key1 > b.key1;

  if (a.key2 != b.key2)
    return abs(a.key2) > abs(b.key2);

  return a.key3.length() > b.key3.length();
}

int main() {
  std::vector<DataStruct> vector;
  DataStruct dstr1, dstr2, dstr3;

  dstr1.key1 = 1;
  dstr2.key1 = 20;
  dstr3.key1 = 300;

  dstr1.key2 = std::complex(1, 0);
  dstr2.key2 = std::complex(20, 0);
  dstr3.key2 = std::complex(300, 0);

  dstr1.key3 = "lor";
  dstr2.key3 = "lorem";
  dstr3.key3 = "lorem ipsum";

  vector = {dstr2, dstr3, dstr1};
  std::sort(vector.begin(), vector.end(), comp);

  return 0;
}