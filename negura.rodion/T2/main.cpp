#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "DataStruct.h"

using namespace nspace;

int main() {
    std::vector<DataStruct> data;
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        DataStruct ds;

        if (iss >> ds) {
            data.push_back(ds);
        }
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
