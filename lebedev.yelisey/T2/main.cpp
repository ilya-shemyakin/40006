#include "DataStruct.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>

int main() {
    using namespace nspace;

    std::vector<DataStruct> data;

    while (!std::cin.eof()) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
        }
    }

    std::sort(data.begin(), data.end(), compare);

    std::copy(data.begin(), data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
