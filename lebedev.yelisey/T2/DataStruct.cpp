#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <iterator>
#include <cmath>

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    if (!std::getline(in, line)) {
        return in;
    }

    data = DataStruct{};
    bool has_key1 = false, has_key2 = false, has_key3 = false;

    size_t key1_pos = line.find(":key1 ");
    size_t key2_pos = line.find(":key2 #c(");
    size_t key3_pos = line.find(":key3 \"");

    if (key1_pos == std::string::npos || key2_pos == std::string::npos || key3_pos == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        size_t key1_end = line.find(':', key1_pos + 6);
        std::string key1_str = line.substr(key1_pos + 6, key1_end - (key1_pos + 6));
        if (key1_str.size() >= 2 && (key1_str.substr(key1_str.size() - 2) == "ll" || key1_str.substr(key1_str.size() - 2) == "LL")) {
            key1_str = key1_str.substr(0, key1_str.size() - 2);
            data.key1 = std::stoll(key1_str);
            has_key1 = true;
        }

        size_t key2_end = line.find(')', key2_pos + 9);
        std::string complex_str = line.substr(key2_pos + 9, key2_end - (key2_pos + 9));
        size_t space_pos = complex_str.find(' ');
        if (space_pos != std::string::npos) {
            double real = std::stod(complex_str.substr(0, space_pos));
            double imag = std::stod(complex_str.substr(space_pos + 1));
            data.key2 = std::complex<double>(real, imag);
            has_key2 = true;
        }

        size_t key3_end = line.find('"', key3_pos + 7);
        data.key3 = line.substr(key3_pos + 7, key3_end - (key3_pos + 7));
        has_key3 = true;

    }
    catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!has_key1 || !has_key2 || !has_key3) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << data.key1 << "ll"
        << ":key2 #c(" << std::fixed << std::setprecision(1)
        << data.key2.real() << " " << data.key2.imag()
        << "):key3 \"" << data.key3 << "\":)";
    return out;
}

void processData() {
    std::vector<DataStruct> dataVector;

    std::copy(std::istream_iterator<DataStruct>(std::cin), std::istream_iterator<DataStruct>(), std::back_inserter(dataVector));

    std::sort(dataVector.begin(), dataVector.end(), [](const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }

        double abs_a = std::abs(a.key2);
        double abs_b = std::abs(b.key2);
        if (std::abs(abs_a - abs_b) > 1e-10) {
            return abs_a < abs_b;
        }

        return a.key3.length() < b.key3.length();
        });

    std::copy(dataVector.begin(), dataVector.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));
}
