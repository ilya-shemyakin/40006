#include "DataStruct.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <clocale>
#include <limits>
int main()
{
    std::vector<DataStruct> data;
    bool flag = true;
    while (std::cin.good()) {
        DataStruct Tdata;

        std::cin >> Tdata;
        if (std::cin.eof()) {
            flag = false;
        }

        if (std::cin.fail() && flag) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if (flag) {
            data.push_back(Tdata);
        }
    }
    if (data.empty()) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
        return 0;
    }
    std::cout << std::fixed << std::setprecision(1);
    std::sort(data.begin(), data.end());
    auto it = data.begin();
    while (it != data.end()) {
        std::cout << *it << '\n';
        ++it;
    }

    return 0;
}
