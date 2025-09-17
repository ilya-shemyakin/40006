#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <string>
#include <complex>
#include <iosfwd>

using Key1 = unsigned long long;
using Key2 = std::complex<double>;

struct DataStruct {
    Key1        key1;
    Key2        key2;
    std::string key3;
    std::string keyT;
};

bool operator==(const DataStruct& a, const DataStruct& b);
bool operator!=(const DataStruct& a, const DataStruct& b);
bool operator>(const DataStruct& a, const DataStruct& b);
bool operator<=(const DataStruct& a, const DataStruct& b);
bool operator<(const DataStruct& a, const DataStruct& b);
bool operator>=(const DataStruct& a, const DataStruct& b);


std::istream& operator>>(std::istream& in, DataStruct& v);
std::ostream& operator<<(std::ostream& out, const DataStruct& v);
#endif
