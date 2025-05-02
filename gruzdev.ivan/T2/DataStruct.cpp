#include <iostream>
#include <string>
#include "DataStruct.h"

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, CharIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        char quote = '0';
        in >> quote;
        if (quote != '\'') {
            in.setstate(std::ios::failbit);
            return in;
        }

        in >> dest.ref;

        in >> quote;
        if (quote != '\'') {
            in.setstate(std::ios::failbit);
            return in;
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, RationalIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> LabelIO{ "N" };
        long long numerator = 0;
        in >> numerator;
        in >> DelimiterIO{ ':' } >> LabelIO{ "D" };
        unsigned long long denominator = 0;
        in >> denominator;
        in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };

        if (in && denominator != 0) {
            dest.ref = { numerator, denominator };
        }
        else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        std::string data = "";
        if ((in >> data) && (data != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        DataStruct input;
        bool flag_key1 = false;
        bool flag_key2 = false;
        bool flag_key3 = false;

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

        while (in) {
            if (in.peek() == ')') {
                in.ignore();
                break;
            }

            std::string field;

            if (!(in >> field)) {
                break;
            }

            if (field == "key1") {
                in >> CharIO{ input.key1 } >> DelimiterIO{ ':' };
                flag_key1 = true;
            }
            else if (field == "key2") {
                in >> RationalIO{ input.key2 } >> DelimiterIO{ ':' };
                flag_key2 = true;
            }
            else if (field == "key3") {
                in >> StringIO{ input.key3 } >> DelimiterIO{ ':' };
                flag_key3 = true;
            }
            else {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in && flag_key1 && flag_key2 && flag_key3) {
            dest = input;
        }
        else {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        std::ostream::sentry sentry(out);
        if (!sentry) {
            return out;
        }

        iofmtguard fmtguard(out);
        out << "(:key1 '" << src.key1 << "':key2 (:N " << src.key2.first
            << ":D " << src.key2.second << ":):key3 \"" << src.key3 << "\":)";
        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags()) {
    }

    iofmtguard::~iofmtguard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    bool compare(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        double a_rational = static_cast<double>(a.key2.first) / a.key2.second;
        double b_rational = static_cast<double>(b.key2.first) / b.key2.second;
        if (a_rational != b_rational) {
            return a_rational < b_rational;
        }
        return a.key3.length() < b.key3.length();
    }
}
