﻿#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include "DataStruct.h"

int main() {
    using nspace::DataStruct;
    using nspace::compareData;
    std::vector< DataStruct > data;
    while (!std::cin.eof()) {
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
        std::copy(
            std::istream_iterator< DataStruct >(std::cin),
            std::istream_iterator< DataStruct >(),
            std::back_inserter(data)
        );
    }
    std::sort(data.begin(), data.end(), compareData);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator< DataStruct >(std::cout, "\n")
    );

    return 0;
}

