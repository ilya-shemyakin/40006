#include "DataStruct.h"

#include <iostream>
#include <string>
#include <complex>
#include <cctype>
#include <cmath>
#include <sstream>   // нужно для std::istringstream

using namespace std;

bool operator==(DataStruct& a, DataStruct& b) {
    return ((a.key1 == b.key1) && (abs(a.key2) == abs(b.key2)) && (a.key3.length() == b.key3.length()));
}

bool operator!=(DataStruct& a, DataStruct& b) {
    return !(a == b);
}

bool operator>(DataStruct& a, DataStruct& b) {
    if (a.key1 > b.key1)
        return true;
    else if (a.key1 == b.key1) {
        if (abs(a.key2) > abs(b.key2))
        {
            return true;
        }
        else if (abs(a.key2) == abs(b.key2))
        {
            if (a.key3.length() > b.key3.length())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else {
        return false;
    }
}

bool operator<=(DataStruct& a, DataStruct& b) {
    return !(a > b);
}

bool operator<(DataStruct& a, DataStruct& b) {
    if (a.key1 < b.key1)
        return true;
    else if (a.key1 == b.key1) {
        if (abs(a.key2) < abs(b.key2))
        {
            return true;
        }
        else if (abs(a.key2) == abs(b.key2))
        {
            if (a.key3.length() < b.key3.length())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else {
        return false;
    }
}

bool operator>=(DataStruct& a, DataStruct& b) {
    return !(a < b);
}

std::istream& operator>>(std::istream& in, DataStruct& v) {
    std::string temp, instr, k1, k2, k3, k2_1, k2_2;

    int pos;
    getline(in, instr);
    if (instr.empty()) {
        in.setstate(ios::eofbit);
        return in;
    }
    temp = instr;

    temp.erase(0, 2);
    for (int i = 0; i < 3; i++)
    {
        pos = temp.find(':');
        if (temp[3] == '3') {
            k3 = temp.substr(5, pos - 5);
            if (k3[0] == '"' && k3.find('"', 1) != string::npos && k3.length() > 2) {
                v.key3 = k3;

            }
            else {
                in.setstate(std::ios::failbit);
            }

        }
        else if (temp[3] == '2') {
            k2 = temp.substr(5, pos - 5);
            if (k2.find("#c(") != string::npos) {
                k2 = k2.substr(3, k2.length() - 4);

                std::istringstream iss(k2);
                double re = 0.0, im = 0.0;
                iss >> re >> im;
                v.key2 = { re,im };

            }
            else {
                in.setstate(std::ios::failbit);
            }

        }
        else if (temp[3] == '1') {
            k1 = temp.substr(5, pos - 5);

            string word;
            word = k1.find("ull") != string::npos ? "ull" : k1.find("ULL") != string::npos ? "ULL" : "";

            if (word == "") {
                in.setstate(std::ios::failbit);
            }
            else {

                if (k1.length() > 3) {
                    v.key1 = std::stoull(k1);
                }
                else {
                    in.setstate(std::ios::failbit);
                }
            }
        }

        if (!cin.good())
            break;
        temp.erase(0, pos + 1);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& v) {
    out << "(:key1 " << v.key1 << "ull:key2 #c(" << v.key2.real() << ' ' << v.key2.imag() << "):key3 " << v.key3 << ":)";
    return out;
}
