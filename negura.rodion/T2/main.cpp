#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "DataStruct.h"

using namespace nspace;

int main() {
  std::vector<DataStruct> data;

  std::istream_iterator<DataStruct> it(std::cin), end;
  std::copy(it, end, std::back_inserter(data));

  if (data.empty()) {
    std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
    return 0;
  }

  std::sort(data.begin(), data.end(), compareDataStructs);

  std::ostream_iterator<DataStruct> out_it(std::cout, "\n");
  std::copy(data.begin(), data.end(), out_it);

  return 0;
}
