#include "DataStruct.h"
#include <algorithm>
#include <complex>
#include <iostream>
#include <string>
#include <vector>

bool comp(DataStruct a, DataStruct b) {
  if (a.key1 != b.key1)
    return a.key1 < b.key1;

  if (a.key2 != b.key2)
    return abs(a.key2) < abs(b.key2);

  return a.key3.length() < b.key3.length();
}

int main() {
  DataStruct dstr1, dstr2, dstr3;

  dstr1.key1 = 1;
  dstr2.key1 = 20;
  dstr3.key1 = 300;

  dstr1.key2 = std::complex<double>(1, 0);
  dstr2.key2 = std::complex<double>(20, 0);
  dstr3.key2 = std::complex<double>(300, 0);

  dstr1.key3 = "lor";
  dstr2.key3 = "lorem";
  dstr3.key3 = "lorem ipsum";

  std::vector<DataStruct> vector = {dstr3, dstr2, dstr1};

  for (int i = 0; i < vector.size(); i++) {
    std::cout << vector[i] << "\n";
  }
  std::cout << "\n";

  std::sort(vector.begin(), vector.end(), comp);

  for (int i = 0; i < vector.size(); i++) {
    std::cout << vector[i] << "\n";
  }

  return 0;
}