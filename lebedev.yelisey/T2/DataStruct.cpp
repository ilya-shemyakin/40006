#include "DataStruct.h"
#include <iomanip>
#include <sstream>

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    if (!std::getline(in, line)) {
        return in;
    }

    size_t pos1 = line.find(":key1 ");
    size_t pos2 = line.find(":key2 #c(");
    size_t pos3 = line.find(":key3 \"");
    size_t pos4 = line.find("\":)");

    if (pos1 == std::string::npos || pos2 == std::string::npos ||
        pos3 == std::string::npos || pos4 == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        std::string key1Str = line.substr(pos1 + 6, line.find(':', pos1 + 1) - (pos1 + 6));
        data.key1 = std::stoll(key1Str);

        std::string key2Str = line.substr(pos2 + 9, line.find(')', pos2) - (pos2 + 9));
        std::istringstream iss(key2Str);
        double real, imag;
        iss >> real >> imag;
        data.key2 = std::complex<double>(real, imag);

        data.key3 = line.substr(pos3 + 7, pos4 - (pos3 + 7));
    }
    catch (...) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << data.key1
        << ":key2 #c(" << std::fixed << std::setprecision(1)
        << data.key2.real() << " " << data.key2.imag()
        << "):key3 \"" << data.key3 << "\":)";
    return out;
}

bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (std::abs(a.key2) != std::abs(b.key2)) return std::abs(a.key2) < std::abs(b.key2);
    return a.key3.length() < b.key3.length();
}

void processData() {
    std::vector<DataStruct> dataVector;
    DataStruct temp;

    while (std::cin >> temp) {
        dataVector.push_back(temp);
    }

    std::sort(dataVector.begin(), dataVector.end(), compareDataStructs);

    for (const auto& item : dataVector) {
        std::cout << item << std::endl;
    }
}
