#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <iostream>

namespace nspace {
    struct DataStruct {
        double key1;
        unsigned long long key2;
        std::string key3;
    };

    struct DelimiterIO {
        char exp;
    };

    struct DblLitIO {
        double& ref;
    };

    struct UllLitIO {
        unsigned long long& ref;
    };

    struct StringIO {
        std::string& ref;
    };

    class iofmtguard {
    public:
        iofmtguard(std::basic_ios< char >& s);
        ~iofmtguard();
    private:
        std::basic_ios< char >& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios< char >::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DblLitIO&& dest);
    std::istream& operator>>(std::istream& in, UllLitIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    bool compareData(const DataStruct& a, const DataStruct& b);
}
#endif

