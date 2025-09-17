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

    size_t key1_start = line.find(":key1 ");
    size_t key2_start = line.find(":key2 #c(");
    size_t key3_start = line.find(":key3 \"");
    
    if (key1_start == std::string::npos || 
        key2_start == std::string::npos || 
        key3_start == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        size_t key1_end = line.find(':', key1_start + 1);
        std::string key1_str = line.substr(key1_start + 6, key1_end - (key1_start + 6));
        
        if (key1_str.size() >= 2 && 
            (key1_str.substr(key1_str.size() - 2) == "ll" || 
             key1_str.substr(key1_str.size() - 2) == "LL")) {
            key1_str = key1_str.substr(0, key1_str.size() - 2);
        }
        data.key1 = std::stoll(key1_str);

        size_t key2_end = line.find(')', key2_start);
        std::string key2_str = line.substr(key2_start + 9, key2_end - (key2_start + 9));
        std::istringstream iss(key2_str);
        double real, imag;
        iss >> real >> imag;
        data.key2 = std::complex<double>(real, imag);

        size_t key3_end = line.find('"', key3_start + 7);
        data.key3 = line.substr(key3_start + 7, key3_end - (key3_start + 7));
    }
    catch (...) {
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

bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    
    double abs_a = std::abs(a.key2);
    double abs_b = std::abs(b.key2);
    if (std::abs(abs_a - abs_b) > 1e-10) return abs_a < abs_b;
    
    return a.key3.length() < b.key3.length();
}

void processData() {
    std::vector<DataStruct> dataVector;
    
    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(dataVector)
    );

    std::sort(dataVector.begin(), dataVector.end(), compareDataStructs);

    std::copy(
        dataVector.begin(),
        dataVector.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
}

