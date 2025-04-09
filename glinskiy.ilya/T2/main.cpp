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
  std::string input;
  std::vector<DataStruct> vector;

  while (std::cin) {
    std::cin >> input;
    // std::copy(input.find('('), input.find(')'));
  }

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