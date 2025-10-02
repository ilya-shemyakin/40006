#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "DataStruct.h"

int main() {
  std::vector<nspace::DataStruct> data;

  while (!std::cin.eof()) {
    std::copy(
      std::istream_iterator<nspace::DataStruct>(std::cin),
      std::istream_iterator<nspace::DataStruct>(),
      std::back_inserter(data)
    );

    if (std::cin.fail() && !std::cin.eof()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  if (!data.empty()) {
    std::sort(data.begin(), data.end(), nspace::compareDataStruct);
    std::copy(data.begin(), data.end(),
      std::ostream_iterator<nspace::DataStruct>(std::cout, "\n"));
    std::cout << "Atleast one supported record type" << std::endl;
  }
  else {
    std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
  }

  return 0;
}