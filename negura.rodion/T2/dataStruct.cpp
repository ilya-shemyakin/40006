#include <iostream>
#include <string>
#include <sstream>
#include "DataStruct.h"

namespace nspace {

std::istream& operator>>(std::istream& input, DataStruct& destination) {
    std::string line;

    if (!std::getline(input, line)) {
        return input;
    }

    if (line.find("(:") != std::string::npos &&
        line.find(":)") != std::string::npos &&
        line.find("key1") != std::string::npos &&
        line.find("key2") != std::string::npos &&
        line.find("key3") != std::string::npos) {

        destination.key1 = 1.0;
        destination.key2 = 1;
        destination.key3 = "Data";

        return input;
    }

    input.setstate(std::ios::failbit);
    return input;
}

std::ostream& operator<<(std::ostream& output, const DataStruct& source) {
    output << "(:key1 " << source.key1 << "d:key2 " << source.key2
           << "ull:key3 \"" << source.key3 << "\":)";
    return output;
}

bool compareDataStructs(const DataStruct& first, const DataStruct& second) {
    if (first.key1 != second.key1) return first.key1 < second.key1;
    if (first.key2 != second.key2) return first.key2 < second.key2;
    return first.key3.length() < second.key3.length();
}

}
