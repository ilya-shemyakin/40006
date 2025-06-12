//#include <iostream>
//#include <vector>
//#include <iterator>
//#include <algorithm>
//#include <limits>
//#include "DataStruct.h"
//
//bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
//    if (a.key1 != b.key1)
//        return a.key1 < b.key1;
//    if (std::abs(a.key2) != std::abs(b.key2))
//        return std::abs(a.key2) < std::abs(b.key2);
//    return a.key3.length() < b.key3.length();
//}
//
//int main() {
//    std::vector<DataStruct> data;
//    while (std::cin.good()) {
//        std::copy(
//            std::istream_iterator<DataStruct>(std::cin),
//            std::istream_iterator<DataStruct>(),
//            std::back_inserter(data)
//        );
//        if (std::cin.fail() && !std::cin.eof()) {
//            std::cin.clear();
//            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//        }
//    }
//    if (data.empty()) {
//        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
//        return 0;
//    }
//
//    std::sort(data.begin(), data.end(), compareDataStruct);
//
//    std::copy(
//        data.begin(),
//        data.end(),
//        std::ostream_iterator<DataStruct>(std::cout, "\n")
//    );
//}
