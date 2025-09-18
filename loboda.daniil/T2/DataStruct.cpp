#include "DataStruct.h"
#include <sstream>
#include <iostream>
#include <string>
#include <complex>
#include <cctype>
#include <cmath>

bool operator==(const DataStruct& a, const DataStruct& b) {
    return ((a.key1 == b.key1)
        && (abs(a.key2) == abs(b.key2))
        && (a.key3.length() == b.key3.length()));
}

bool operator!=(const DataStruct& a, const DataStruct& b) {
    return !(a == b);
}

bool operator>(const DataStruct& a, const DataStruct& b) {
    if (a.key1 > b.key1)
        return true;
    else if (a.key1 == b.key1) {
        if (abs(a.key2) > abs(b.key2))
        {
            return true;
        }
        else if (abs(a.key2) == abs(b.key2))
            return (a.key3.length() > b.key3.length());

        return false;
    }

    return false;
}

bool operator<=(const DataStruct& a, const DataStruct& b) {
    return !(a > b);
}

bool operator<(const DataStruct& a, const DataStruct& b) {
    if (a.key1 < b.key1)
        return true;
    else if (a.key1 == b.key1) {
        if (abs(a.key2) < abs(b.key2))
        {
            return true;
        }
        else if (abs(a.key2) == abs(b.key2))
        {
            return(a.key3.length() < b.key3.length());

            return false;
        }
    }

    return false;
}

bool operator>=(const DataStruct& a, const DataStruct& b) {
    return !(a < b);
}

std::istream& operator>>(std::istream& in, DataStruct& v) {
    std::string temp, instr, k1, k2, k3, k2_1, k2_2;

    if (!std::getline(in, instr)) {
        return in;
    }
    if (instr.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }
    temp = instr;

    if (temp.size() < 2 || temp[0] != '(' || temp[1] != ':') {
        in.setstate(std::ios::failbit);
        return in;
    }
    temp.erase(0, 2);
    for (int i = 0; i < 3; i++)
    {
        if (temp.size() < 4) { in.setstate(std::ios::failbit); break; }

        if (temp[3] == '3') {

            std::size_t q1 = temp.find('"', 5);
            std::size_t q2 = (q1 == std::string::npos)
                ? std::string::npos : temp.find('"', q1 + 1);
            if (q1 == std::string::npos || q2 == std::string::npos) {
                in.setstate(std::ios::failbit);
            }
            else {
                k3 = temp.substr(q1, q2 - q1 + 1);
                v.key3 = k3;
                std::size_t pos = temp.find(':', q2 + 1);
                if (pos == std::string::npos) {
                    in.setstate(std::ios::failbit);
                }
                else {
                    temp.erase(0, pos + 1);
                }
            }
        }
        else if (temp[3] == '2') {
            std::size_t pos = temp.find(':');
            if (pos == std::string::npos || pos < 5) { in.setstate(std::ios::failbit); break; }

            k2 = temp.substr(5, pos - 5);
            if (k2.find("#c(") != std::string::npos) {
                if (k2.length() < 4) {
                    in.setstate(std::ios::failbit);
                }
                else {
                    k2 = k2.substr(3, k2.length() - 4);

                    std::istringstream iss(k2);
                    double re = 0.0, im = 0.0;
                    iss >> re >> im;
                    v.key2 = { re, im };
                }
            }
            else {
                in.setstate(std::ios::failbit);
            }

            if (!in.fail()) temp.erase(0, pos + 1);
        }
        else if (temp[3] == '1') {
            std::size_t pos = temp.find(':');
            if (pos == std::string::npos || pos < 5) { in.setstate(std::ios::failbit); break; }

            k1 = temp.substr(5, pos - 5);

            std::string word;
            word = k1.find("ull") != std::string::npos ? "ull"
                : k1.find("ULL") != std::string::npos ? "ULL" : "";

            if (word == "") {
                in.setstate(std::ios::failbit);
            }
            else {
                if (k1.length() > 3) {
                    try { v.key1 = std::stoull(k1); }
                    catch (...) { in.setstate(std::ios::failbit); }
                }
                else {
                    in.setstate(std::ios::failbit);
                }
            }

            if (!in.fail()) temp.erase(0, pos + 1);
        }

        if (!in.good())
            break;
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& v) {
    out << "(:key1 " << v.key1 << "ull:key2 #c(" << v.key2.real()
        << ' ' << v.key2.imag() << "):key3 " << v.key3 << ":)";
    return out;
}
