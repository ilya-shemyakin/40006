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

    size_t pos = 0;
    while (pos < line.size()) {
        size_t field_start = line.find(':', pos);
        if (field_start == std::string::npos) break;

        size_t space_pos = line.find(' ', field_start);
        if (space_pos == std::string::npos) break;

        std::string field_name = line.substr(field_start + 1, space_pos - field_start - 1);

        if (field_name == "key1" && !has_key1) {
            size_t value_end = line.find(':', space_pos + 1);
            if (value_end == std::string::npos) break;

            std::string value_str = line.substr(space_pos + 1, value_end - space_pos - 1);

            if (value_str.size() >= 2 && (value_str.substr(value_str.size() - 2) == "ll" || value_str.substr(value_str.size() - 2) == "LL")) {
                value_str = value_str.substr(0, value_str.size() - 2);
                try {
                    data.key1 = std::stoll(value_str);
                    has_key1 = true;
                }
                catch (...) {
                }
            }
            pos = value_end;
        }
        else if (field_name == "key2" && !has_key2) {
            if (line.substr(space_pos + 1, 3) != "#c(") {
                pos = space_pos + 1;
                continue;
            }

            size_t paren_end = line.find(')', space_pos + 4);
            if (paren_end == std::string::npos) break;

            std::string value_str = line.substr(space_pos + 4, paren_end - space_pos - 4);
            std::istringstream iss(value_str);
            std::string real_str, imag_str;

            if (iss >> real_str >> imag_str) {
                try {
                    double real = std::stod(real_str);
                    double imag = std::stod(imag_str);
                    data.key2 = std::complex<double>(real, imag);
                    has_key2 = true;
                }
                catch (...) {
                }
            }
            pos = paren_end + 1;
        }
        else if (field_name == "key3" && !has_key3) {
            if (line[space_pos + 1] != '"') {
                pos = space_pos + 1;
                continue;
            }

            size_t quote_end = space_pos + 2;
            while (quote_end < line.size() && line[quote_end] != '"') {
                quote_end++;
            }

            if (quote_end < line.size()) {
                data.key3 = line.substr(space_pos + 2, quote_end - space_pos - 2);
                has_key3 = true;
                pos = quote_end + 1;
            }
            else {
                break;
            }
        }
        else {
            size_t next_colon = line.find(':', space_pos + 1);
            if (next_colon == std::string::npos) break;
            pos = next_colon;
        }
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
