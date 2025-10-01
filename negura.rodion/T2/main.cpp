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

  std::copy(
    std::istream_iterator<DataStruct>(std::cin),
    std::istream_iterator<DataStruct>(),
    std::back_inserter(data)
  );

  if (data.empty())
  {
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
