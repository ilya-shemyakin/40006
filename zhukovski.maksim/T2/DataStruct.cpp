#include <algorithm>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <utility>
#include <iomanip>
#include "DataStruct.h"
#include <cstring>
#include <cctype>

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c = '0';
        in >> c;
        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, DblLitIO&& lit) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        double value = 0.0;
        in >> value;
        if (!in) {
            return in;
        }
        std::string s;
        std::getline(in, s, ':');
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return std::tolower(static_cast<unsigned char>(c)); });
        if (s != "d") {
            in.setstate(std::ios::failbit);
            return in;
        }
        in.unget();
        lit.num = value;
        return in;
    }

    std::istream& operator>>(std::istream& in, SllLitIO&& lit) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        long long value = 0;
        in >> value;
        if (!in) {
            return in;
        }
        std::string s;
        std::getline(in, s, ':');
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return std::tolower(static_cast<unsigned char>(c)); });
        if (s != "ll") {
            in.setstate(std::ios::failbit);
            return in;
        }
        lit.num = value;
        in.unget();
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        DataStruct input;
        bool f1 = false;
        bool f2 = false;
        bool f3 = false;
        bool valid = true;

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
        if (!in) {
            return in;
        }

        while (true && in.peek() != ')') {
            std::string field_name;
            in >> field_name;
            if (!in) {
                break;
            }
            bool field_parsed = false;
            if (field_name == "key1" && !f1) {
                in >> DblLitIO{ input.key1 } >> DelimiterIO{ ':' };
                if (in) {
                    f1 = true;
                    field_parsed = true;
                }
            }
            else if (field_name == "key2" && !f2) {
                in >> SllLitIO{ input.key2 } >> DelimiterIO{ ':' };
                if (in) {
                    f2 = true;
                    field_parsed = true;
                }
            }
            else if (field_name == "key3" && !f3) {
                in >> StringIO{ input.key3 } >> DelimiterIO{ ':' };
                if (in) {
                    f3 = true;
                    field_parsed = true;
                }
            }
            if (!field_parsed) {
                valid = false;
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in.fail()) {
            in.clear();
        }
        in >> DelimiterIO{ ')' };
        if (in && valid && f1 && f2 && f3) {
            dest = input;
        }
        else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& dest) {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1) << "(:key1 " << dest.key1 << "d:key2 " << dest.key2 << "ll:key3 \"" << dest.key3 << "\":)";
        return out;
    }

    bool compare(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }
        return a.key3.size() < b.key3.size();
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {
    }
    iofmtguard::~iofmtguard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
}
