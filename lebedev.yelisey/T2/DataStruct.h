#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <string>

struct DataStruct {
    long long key1;
    std::complex<double> key2;
    std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);
void processData();

#endif 