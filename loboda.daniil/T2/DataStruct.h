#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <complex>
#include <iostream>
#include <string>

struct DataStruct
{
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

struct DelimiterIO
{
    char exp;
};

struct UnsignedLongLongIO
{
    unsigned long long& ref;
};

struct ComplexIO
{
    std::complex<double>& ref;
};

struct StringIO
{
    std::string& ref;
};

{
public:
    iofmtguard(std::basic_ios<char>&s);
    ~iofmtguard();
private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest);
std::istream& operator>>(std::istream& in, ComplexIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& src);

#endif
