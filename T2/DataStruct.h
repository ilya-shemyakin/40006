#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <complex>

namespace task01 {

    struct DataStruct {
        unsigned long long   key1 = 0ull;
        std::complex<double> key2{ 0.0, 0.0 };
        std::string          key3;
    };

    class iofmtguard {
    public:
        explicit iofmtguard(std::basic_ios<char>& s);
        ~iofmtguard();
    private:
        std::basic_ios<char>& s_;
        std::streamsize       width_;
        char                  fill_;
        std::streamsize       precision_;
        std::ios::fmtflags    fmt_;
    };

    struct Comparator {
        bool operator()(const DataStruct& lhs,
            const DataStruct& rhs) const noexcept;
    };

    std::istream& operator>>(std::istream& in, DataStruct& dst);
    std::ostream& operator<<(std::ostream& out, const DataStruct& s);

}

#endif
