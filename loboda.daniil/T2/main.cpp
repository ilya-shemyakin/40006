#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <clocale>     // для setlocale
#include "DataStruct.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Ru");
    vector<DataStruct> data;
    bool flag = true;
    while (std::cin.good()) {
        DataStruct Tdata;

        cin >> Tdata;
        if (cin.eof()) {
            flag = false;
        }

        if (std::cin.fail() && flag) {
            std::cin.clear();
        }
        else if (flag) {
            data.push_back(Tdata);
        }
    }
    if (data.empty()) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
        return 0;
    }
    cout << fixed << setprecision(1);
    std::sort(data.begin(), data.end());
    auto it = data.begin();
    while (it != data.end()) {
        cout << *it << '\n';
        ++it;
    }

    return 0;
}
