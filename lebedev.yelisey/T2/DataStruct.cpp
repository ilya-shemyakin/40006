#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <iterator>
#include <cmath>

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    while (std::getline(in, line)) {

        if (line.find(":key1 ") == std::string::npos) continue;
        if (line.find(":key2 #c(") == std::string::npos) continue;
        if (line.find(":key3 \"") == std::string::npos) continue;

        size_t key1_start = line.find(":key1 ");
        size_t key1_end = line.find(':', key1_start + 1);
        std::string key1_str = line.substr(key1_start + 6, key1_end - (key1_start + 6));

        if (key1_str.size() < 2 || (key1_str.substr(key1_str.size() - 2) != "ll" && key1_str.substr(key1_str.size() - 2) != "LL")) {
            continue;
        }

        try {
            key1_str = key1_str.substr(0, key1_str.size() - 2);
            data.key1 = std::stoll(key1_str);

            size_t key2_start = line.find(":key2 #c(");
            size_t key2_end = line.find(')', key2_start);
            std::string key2_str = line.substr(key2_start + 9, key2_end - (key2_start + 9));
            std::istringstream iss(key2_str);
            std::string real_str, imag_str;
            iss >> real_str >> imag_str;
            double real = std::stod(real_str);
            double imag = std::stod(imag_str);
            data.key2 = std::complex<double>(real, imag);

            size_t key3_start = line.find(":key3 \"");
            size_t key3_end = key3_start + 7;
            int quote_count = 0;
            while (key3_end < line.size() && quote_count < 2) {
                if (line[key3_end] == '"') quote_count++;
                key3_end++;
            }
            data.key3 = line.substr(key3_start + 7, key3_end - (key3_start + 7) - 1);

            break;
        }
        catch (...) {
            continue;
        }
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

    std::copy(std::istream_iterator<DataStruct>(std::cin), std::istream_iterator<DataStruct>(), std::back_inserter(dataVector)
    );

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
        }
    );

    std::copy(dataVector.begin(), dataVector.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));
}
