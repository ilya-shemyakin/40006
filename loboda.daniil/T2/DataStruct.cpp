#include "DataStruct.h"
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <sstream>

namespace task01
{
    iofmtguard::iofmtguard(std::basic_ios<char>& s)
        : s_(s)
        , width_(s.width())
        , fill_(s.fill())
        , precision_(s.precision())
        , fmt_(s.flags())
    {
    }

    iofmtguard::~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    struct DelimIO { char exp; };
    struct ULLLitIO { unsigned long long& ref; };
    struct ComplexIO { std::complex<double>& r; };
    struct StringIO { std::string& ref; };
    struct FieldLabelIO { std::string  exp; };

    std::istream& operator>>(std::istream& in, DelimIO&& d)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char c = '\0';
        in >> c;
        if (in && c != d.exp) in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, ULLLitIO&& d)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        unsigned long long value = 0ull;
        if (!(in >> value)) return in;

        char u = '\0', l1 = '\0', l2 = '\0';
        in >> u >> l1 >> l2;
        if (!in
            || std::tolower(u) != 'u'
            || std::tolower(l1) != 'l'
            || std::tolower(l2) != 'l')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        d.ref = value;
        return in;
    }

    std::istream& operator>>(std::istream& in, ComplexIO&& d)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char hash = '\0', cchar = '\0', lpar = '\0';
        in >> hash >> cchar >> lpar;
        if (!in || hash != '#' || std::tolower(cchar) != 'c' || lpar != '(')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        double re = 0.0, im = 0.0;
        if (!(in >> re >> im))
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        char rpar = '\0';
        in >> rpar;
        if (!in || rpar != ')') in.setstate(std::ios::failbit);

        d.r = { re, im };
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& d)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char quote = '\0';
        in >> quote;
        if (!in || quote != '"')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::getline(in, d.ref, '"');
        return in;
    }

    std::istream& operator>>(std::istream& in, FieldLabelIO&& d)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        std::string tmp;
        if (!(in >> tmp)) return in;
        if (tmp != d.exp) in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dst)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct tmp;
        bool gotK1 = false, gotK2 = false, gotK3 = false;

        using sep = DelimIO;
        using k1io = ULLLitIO;
        using k2io = ComplexIO;
        using strio = StringIO;

        if (!(in >> sep{ '(' } >> sep{ ':' })) return in;

        while (in)
        {
            std::string field;
            if (!(in >> field)) return in;

            if (field == "key1")
            {
                if (gotK1 || !(in >> sep{ ' ' } >> k1io{ tmp.key1 })) break;
                gotK1 = true;
            }
            else if (field == "key2")
            {
                if (gotK2 || !(in >> sep{ ' ' } >> k2io{ tmp.key2 })) break;
                gotK2 = true;
            }
            else if (field == "key3")
            {
                if (gotK3 || !(in >> sep{ ' ' } >> strio{ tmp.key3 })) break;
                gotK3 = true;
            }
            else
            {
                in.setstate(std::ios::failbit);
                break;
            }

            char tail = '\0';
            in >> tail;
            if (!in) break;
            if (tail == ':')        continue;
            else if (tail == ')')
            {
                if (gotK1 && gotK2 && gotK3) dst = tmp;
                else                         in.setstate(std::ios::failbit);
                return in;
            }
            else
            {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& s)
    {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        iofmtguard g(out);
        out << "(:key1 " << s.key1 << "ull:";
        out << "key2 #c(" << std::fixed << std::setprecision(1)
            << s.key2.real() << ' ' << s.key2.imag() << "):";
        out << "key3 \"" << s.key3 << "\":)";
        return out;
    }

    // == Компаратор ==
    bool Comparator::operator()(const DataStruct& lhs,
        const DataStruct& rhs) const noexcept
    {
        if (lhs.key1 != rhs.key1)
            return lhs.key1 < rhs.key1;
        double a = std::abs(lhs.key2), b = std::abs(rhs.key2);
        if (a != b)
            return a < b;
        return lhs.key3.size() < rhs.key3.size();
    }

}
