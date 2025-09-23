#include "DataStruct.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <clocale>
int main()
{
	std::vector<DataStruct> data;

	while (std::cin.good())
	{
		DataStruct input;
		bool flag = true;

		std::cin >> input;
		if (std::cin.eof())
		{
			flag = false;
		}

		if (std::cin.fail() && flag)
		{
			std::cin.clear();
		}
		else if (flag)
		{
			data.push_back(input);
		}
	}
	if (data.empty())
	{
		std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
		return 0;
	}
	std::cout << std::fixed << std::setprecision(1);
	std::sort(data.begin(), data.end());

	auto it = data.begin();
	while (it != data.end())
	{
		std::cout << *it << '\n';
		++it;
	}

	return 0;
}
