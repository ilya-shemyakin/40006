#include "DataStruct.h"
#include <sstream>
#include <iostream>
#include <string>
#include <complex>
#include <cctype>
#include <cmath>
namespace
{
	// Константы для замены магических чисел
	const int KEY_INDEX = 3;
	const int PREFIX_LENGTH = 2;          // Длина префикса "(:"
	const int EXPECTED_KEYS_COUNT = 3;    // Количество ожидаемых ключей
	const int MIN_ULL_LENGTH = 3;
	const int MIN_HEADER_LENGTH = 4;      // Минимальная длина заголовка
	const int DATA_START_POSITION = 5;    // Начало данных ключа
	const int MIN_COMPLEX_LENGTH = 4;     // Минимальная длина комплексного числа
	const int COMPLEX_PREFIX_LENGTH = 3;  // Длина префикса "#c("
	const int COMPLEX_SUFFIX_LENGTH = 1;  // Длина суфикса ")"

	const char COLON = ':';
	const char QUOTE = '"';
}

bool operator==(const DataStruct& a, const DataStruct& b)
{
	return ((a.key1_ == b.key1_)
		&& (std::abs(a.key2_) == std::abs(b.key2_))
		&& (a.key3_.length() == b.key3_.length()));
}
bool operator!=(const DataStruct& a, const DataStruct& b)
{
	return !(a == b);
}
bool operator>(const DataStruct& a, const DataStruct& b)
{
	if (a.key1_ > b.key1_)
	{
		return true;
	}
	else if (a.key1_ == b.key1_)
	{
		if (std::abs(a.key2_) > std::abs(b.key2_))
		{
			return true;
		}
		else if (std::abs(a.key2_) == std::abs(b.key2_))
		{
			return (a.key3_.length() > b.key3_.length());
		}
	}
	return false;
}
bool operator<=(const DataStruct& a, const DataStruct& b)
{
	return !(a > b);
}
bool operator<(const DataStruct& a, const DataStruct& b)
{
	return (b > a);
}
bool operator>=(const DataStruct& a, const DataStruct& b)
{
	return !(a < b);
}

std::istream& operator>>(std::istream& in, DataStruct& v)
{
	std::string temp, instr, k1, k2, k3;
	bool got1 = false;
	bool got2 = false;
	bool got3 = false;

	if (!std::getline(in, instr))
	{
		return in;
	}
	if (instr.empty())
	{
		in.setstate(std::ios::failbit);
		return in;
	}
	temp = instr;

	if (temp.size() < PREFIX_LENGTH || temp[0] != '(' || temp[1] != COLON)
	{
		in.setstate(std::ios::failbit);
		return in;
	}

	temp.erase(0, PREFIX_LENGTH);
	for (int i = 0; i < EXPECTED_KEYS_COUNT; i++)
	{
		if (temp.size() < MIN_HEADER_LENGTH)
		{
			in.setstate(std::ios::failbit);
			break;
		}

		if (temp[KEY_INDEX] == '3')
		{
			std::size_t q1 = temp.find(QUOTE, DATA_START_POSITION);
			std::size_t q2 = (q1 == std::string::npos)
				? std::string::npos : temp.find(QUOTE, q1 + 1);
			if (q1 == std::string::npos || q2 == std::string::npos)
			{
				in.setstate(std::ios::failbit);
			}
			else
			{
				k3 = temp.substr(q1, q2 - q1 + 1);
				v.key3_ = k3;
				got3 = true;
				std::size_t pos = temp.find(COLON, q2 + 1);
				if (pos == std::string::npos)
				{
					in.setstate(std::ios::failbit);
				}
				else
				{
					temp.erase(0, pos + 1);
				}
			}
		}

		else if (temp[KEY_INDEX] == '2')
		{
			std::size_t pos = temp.find(COLON);
			if (pos == std::string::npos || pos < DATA_START_POSITION)
			{
				in.setstate(std::ios::failbit);
				break;
			}

			k2 = temp.substr(DATA_START_POSITION, pos - DATA_START_POSITION);
			if (k2.find("#c(") != std::string::npos)
			{
				if (k2.length() < MIN_COMPLEX_LENGTH)
				{
					in.setstate(std::ios::failbit);
				}
				else
				{
					k2 = k2.substr(COMPLEX_PREFIX_LENGTH, k2.length() - COMPLEX_PREFIX_LENGTH - COMPLEX_SUFFIX_LENGTH);
					std::istringstream iss(k2);
					double re = 0.0, im = 0.0;
					if (!(iss >> re >> im))
					{
						in.setstate(std::ios::failbit);
					}
					else
					{
						v.key2_ = { re, im };
						got2 = true;
					}
				}
			}
			else
			{
				in.setstate(std::ios::failbit);
			}

			if (!in.fail())
			{
				temp.erase(0, pos + 1);
			}
		}

		else if (temp[KEY_INDEX] == '1')
		{
			std::size_t pos = temp.find(COLON);
			if (pos == std::string::npos || pos < DATA_START_POSITION)
			{
				in.setstate(std::ios::failbit);
				break;
			}

			k1 = temp.substr(DATA_START_POSITION, pos - DATA_START_POSITION);

			std::string word;
			word = k1.find("ull") != std::string::npos ? "ull"
				: k1.find("ULL") != std::string::npos ? "ULL" : "";

			if (word == "")
			{
				in.setstate(std::ios::failbit);
			}
			else
			{
				if (k1.length() > MIN_ULL_LENGTH)
				{
					try
					{
						v.key1_ = std::stoull(k1);
						got1 = true;
					}
					catch (...)
					{
						in.setstate(std::ios::failbit);
					}
				}
				else
				{
					in.setstate(std::ios::failbit);
				}
			}

			if (!in.fail())
			{
				temp.erase(0, pos + 1);
			}
		}
		else
		{
			in.setstate(std::ios::failbit);
		}

		if (!in.good())
		{
			break;
		}
	}

	if (!(got1 && got2 && got3))
	{
		in.setstate(std::ios::failbit);
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& v)
{
	out << "(:key1 " << v.key1_ << "ull:key2 #c(" << v.key2_.real()
		<< ' ' << v.key2_.imag() << "):key3 " << v.key3_ << ":)";
	return out;
}
