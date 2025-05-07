#include "DataStruct.h"
#include <vector>
#include <iterator>
#include <algorithm>

int main() {
    using namespace task01;

    std::vector<DataStruct> data;
    std::copy(std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data));

    std::sort(data.begin(), data.end(), Comparator{});

    std::copy(data.begin(), data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
