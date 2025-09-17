#include "DataStruct.h"
#include <sstream>
#include <iostream>
#include <string>
#include <complex>
#include <cctype>
#include <cmath>

bool operator==(const DataStruct& a, const DataStruct& b) {
	return ((a.key1 == b.key1)
		&& (abs(a.key2) == abs(b.key2))
		&& (a.key3.length() == b.key3.length()));
}

bool operator!=(const DataStruct& a, const DataStruct& b) {
	return !(a == b);
}

bool operator>(const DataStruct& a, const DataStruct& b) {
	if (a.key1 > b.key1)
		return true;
	else if (a.key1 == b.key1) {
		if (abs(a.key2) > abs(b.key2))
		{
			return true;
		}
		else if (abs(a.key2) == abs(b.key2))
			return (a.key3.length() > b.key3.length());

		return false;
	}

	return false;
}

bool operator<=(const DataStruct& a, const DataStruct& b) {
	return !(a > b);
}

bool operator<(const DataStruct& a, const DataStruct& b) {
	if (a.key1 < b.key1)
		return true;
	else if (a.key1 == b.key1) {
		if (abs(a.key2) < abs(b.key2))
		{
			return true;
		}
		else if (abs(a.key2) == abs(b.key2))
		{
			return(a.key3.length() < b.key3.length());

			return false;
		}
	}

	return false;
}

bool operator>=(const DataStruct& a, const DataStruct& b) {
	return !(a < b);
}

std::istream& operator>>(std::istream& in, DataStruct& v) {
	std::string temp, instr, k1, k2, k3, k2_1, k2_2;

	std::size_t pos = temp.find(':');
	if (pos == std::string::npos) { in.setstate(std::ios::failbit); return in; }
	getline(in, instr);
	if (instr.empty()) {
		in.setstate(std::ios::failbit);
		return in;
	}
	temp = instr;

	temp.erase(0, 2);
	for (int i = 0; i < 3; i++)
	{
		pos = temp.find(':');
		if (temp[3] == '3') {
			k3 = temp.substr(5, pos - 5);
			if (k3[0] == '"' && k3.find('"', 1) != std::string::npos && k3.length() > 2) {
				v.key3 = k3;

			}
			else {
				in.setstate(std::ios::failbit);
			}

		}
		else if (temp[3] == '2') {
			k2 = temp.substr(5, pos - 5);
			if (k2.find("#c(") != std::string::npos) {
				k2 = k2.substr(3, k2.length() - 4);

				std::istringstream iss(k2);
				double re = 0.0, im = 0.0;
				iss >> re >> im;
				v.key2 = { re,im };

			}
			else {
				in.setstate(std::ios::failbit);
			}

		}
		else if (temp[3] == '1') {
			k1 = temp.substr(5, pos - 5);

			std::string word;
			word = k1.find("ull") != std::string::npos ? "ull" : k1.find("ULL")
				!= std::string::npos ? "ULL" : "";

			if (word == "") {
				in.setstate(std::ios::failbit);
			}
			else {

				if (k1.length() > 3) {
					v.key1 = std::stoull(k1);
				}
				else {
					in.setstate(std::ios::failbit);
				}
			}
		}

		if (!in.good())
			break;
		temp.erase(0, pos + 1);
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& v) {
	out << "(:key1 " << v.key1 << "ull:key2 #c(" << v.key2.real()
		<< ' ' << v.key2.imag() << "):key3 " << v.key3 << ":)";
	return out;
}
