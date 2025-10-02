#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include <fstream>
#include "DataStruct.h"

using namespace nspace;

int main()
{
  std::vector<DataStruct> data;

  try {
    std::copy(
      std::istream_iterator<DataStruct>(std::cin),
      std::istream_iterator<DataStruct>(),
      std::back_inserter(data)
    );
  }
  catch (...) {
  }

  if (!std::cin.eof() && std::cin.fail()) {
    std::cin.clear();
  }

  if (data.empty()) {
    std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
    return 0;
  }

  std::sort(data.begin(), data.end(), compareDataStructs);

  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator<DataStruct>(std::cout, "\n")
  );

  return 0;
}