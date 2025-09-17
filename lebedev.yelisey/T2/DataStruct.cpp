#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <cmath>

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, SllLitIO&& lit) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        long long value = 0;
        in >> value;
        if (!in) return in;

        std::string suffix;
        std::getline(in, suffix, ':');
        if (suffix != "ll" && suffix != "LL") {
            in.setstate(std::ios::failbit);
            return in;
        }
        in.unget();

        lit.num = value;
        return in;
    }

    std::istream& operator>>(std::istream& in, ComplexIO&& cmp) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };

        double real = 0.0, imag = 0.0;
        in >> real >> imag;

        in >> DelimiterIO{ ')' };

        if (in) {
            cmp.cmp = std::complex<double>(real, imag);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct input;
        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

        bool f1 = false, f2 = false, f3 = false;
        std::string field;

        while (in.peek() != ')' && in) {
            in >> field;

            if (field == "key1") {
                in >> SllLitIO{ input.key1 } >> DelimiterIO{ ':' };
                f1 = true;
            }
            else if (field == "key2") {
                in >> ComplexIO{ input.key2 } >> DelimiterIO{ ':' };
                f2 = true;
            }
            else if (field == "key3") {
                in >> StringIO{ input.key3 } >> DelimiterIO{ ':' };
                f3 = true;
            }
            else {
                in.setstate(std::ios::failbit);
            }
        }

        in >> DelimiterIO{ ')' };

        if (f1 && f2 && f3 && in) {
            dest = input;
        }
        else {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& dest) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        iofmtguard guard(out);
        out << "(:key1 " << dest.key1 << "ll"
            << ":key2 #c(" << std::fixed << std::setprecision(1)
            << dest.key2.real() << " " << dest.key2.imag()
            << "):key3 \"" << dest.key3 << "\":)";
        return out;
    }

    bool compare(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;

        double abs_a = std::abs(a.key2);
        double abs_b = std::abs(b.key2);
        if (std::abs(abs_a - abs_b) > 1e-10) return abs_a < abs_b;

        return a.key3.length() < b.key3.length();
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s), width_(s.width()), fill_(s.fill()),
        precision_(s.precision()), fmt_(s.flags()) {
    }

    iofmtguard::~iofmtguard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
}
