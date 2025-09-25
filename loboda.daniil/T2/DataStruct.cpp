#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <limits>

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
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
std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest)
{
	std::istream::sentry sentry(in);
	if (!sentry)
	{
		return in;
	}

	unsigned long long value;
	in >> value;
	if (!in)
	{
		return in;
	}
	char c1 = 0;
	char c2 = 0;
	char c3 = 0;
	in >> c1 >> c2 >> c3;
	if (!((c1 == 'u' || c1 == 'U') && (c2 == 'l' || c2 == 'L') && (c3 == 'l' || c3 == 'L')))
	{
		in.setstate(std::ios::failbit);
	}
	else
	{
		dest.ref = value;
	}
	return in;
}
std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
	std::istream::sentry sentry(in);
	if (!sentry)
	{
		return in;
	}

	in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };

	double re = 0.0;
	double im = 0.0;
	in >> re >> im;

	in >> DelimiterIO{ ')' };

	if (in)
	{
		dest.ref = std::complex<double>(re, im);
	}

	return in;
}
std::istream& operator>>(std::istream& in, StringIO&& dest)
{
	std::istream::sentry sentry(in);
	if (!sentry)
	{
		return in;
	}
	std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
	
	return in;
}
std::istream& operator>>(std::istream& in, DataStruct& dest)
{
	std::istream::sentry sentry(in);
	if (!sentry)
	{
		return in;
	}

	DataStruct input;
	std::string field;
	bool hasKey1 = false;
	bool hasKey2 = false;
	bool hasKey3 = false;
	in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

	while (in >> field)
	{
		if (field == "key1")
		{
			in >> UnsignedLongLongIO{ input.key1 }
			>> DelimiterIO{ ':' };
			if (in)
			{
				hasKey1 = true;
			}
		}
		else if (field == "key2")
		{
			in >> ComplexIO{ input.key2 } >> DelimiterIO{ ':' };
			if (in)
			{
				hasKey2 = true;
			}
		}
		else if (field == "key3")
		{
			in >> StringIO{ input.key3 } >> DelimiterIO{ ':' };
			if (in)
			{
				hasKey3 = true;
			}
		}
		else if (field == ")")
		{
			if (!(hasKey1 && hasKey2 && hasKey3))
			{
				in.setstate(std::ios::failbit);
			}
			break;
		}
		else
		{
			in.setstate(std::ios::failbit);
			break;
		}
	}

	if (in)
	{
		dest = input;
	}

	return in;
}
std::ostream& operator<<(std::ostream& out, const DataStruct& src)
{
	std::ostream::sentry sentry(out);
	if (!sentry)
	{
		return out;
	}

	iofmtguard guard(out);

	out << "(:key1 " << src.key1 << "ull:key2 #c(" << std::fixed << std::setprecision(1)
		<< src.key2.real() << " " << src.key2.imag() << "):key3 \"" << src.key3 << "\":)";
	return out;
}
iofmtguard::iofmtguard(std::basic_ios<char>& s)
	: s_(s),
	width_(s.width()),
	fill_(s.fill()),
	precision_(s.precision()),
	fmt_(s.flags())
{}
iofmtguard::~iofmtguard()
{
	s_.width(width_);
	s_.fill(fill_);
	s_.precision(precision_);
	s_.flags(fmt_);
}