#include <iostream>
#include <string>
#include <iomanip>
#include "DataStruct.h"

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimeterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        char c;
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        return std::getline(in >> DelimeterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        return in >> dest.ref >> DelimeterIO{ 'd' };
    }

    std::istream& operator>>(std::istream& in, UllLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        unsigned long long value;
        in >> value;

        if (!in) {
            return in;
        }

        char u, l1, l2;
        in >> u >> l1 >> l2;

        if (!in || !((u == 'u' && l1 == 'l' && l2 == 'l') ||
                     (u == 'U' && l1 == 'L' && l2 == 'L'))) {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.ref = value;
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        DataStruct temp;
        in >> DelimeterIO{ '(' } >> DelimeterIO{ ':' };

        std::string key;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        while (true) {
            in >> key;
            if (!in) break;

            if (key == "key1") {
                in >> DoubleIO{ temp.key1 } >> DelimeterIO{ ':' };
                hasKey1 = true;
            }
            else if (key == "key2") {
                in >> UllLitIO{ temp.key2 } >> DelimeterIO{ ':' };
                hasKey2 = true;
            }
            else if (key == "key3") {
                in >> StringIO{ temp.key3 } >> DelimeterIO{ ':' };
                hasKey3 = true;
            }
            else if (key == ")") {
                break;
            }
            else {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in && hasKey1 && hasKey2 && hasKey3) {
            dest = temp;
        }
        else {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
        std::ostream::sentry sentry(out);
        if (!sentry) {
            return out;
        }

        iofmtguard guard(out);
        out << "(:key1 " << std::fixed << std::setprecision(1) << data.key1 << "d"
            << ":key2 " << data.key2 << "ull"
            << ":key3 \"" << data.key3 << "\":)";
        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s), width_(s.width()), fill_(s.fill()),
        precision_(s.precision()), fmt_(s.flags())
    {}

    iofmtguard::~iofmtguard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
    }
}
