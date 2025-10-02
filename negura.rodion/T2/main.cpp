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

DataStruct temp;
while (std::cin >> temp) {
data.push_back(temp);
std::cin >> std::ws;
}

if (!std::cin.eof()) {
std::cin.clear();
}

if (data.empty()) {
std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
return 0;
}

std::cout << "Atleast one supported record type" << std::endl;

std::sort(data.begin(), data.end(), compareDataStructs);

for (const auto& item : data) {
std::cout << item << std::endl;
}

return 0;
}
