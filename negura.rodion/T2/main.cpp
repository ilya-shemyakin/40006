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

  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    DataStruct temp;
    if (std::cin >> temp)
    {
      data.push_back(temp);
    }
  }

  if (data.empty())
  {
    std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
    return 0;
  }

  std::cout << "Atleast one supported record type" << std::endl;
  return 0;
}
