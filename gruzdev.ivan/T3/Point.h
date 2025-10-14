#ifndef POINT_H_
#define POINT_H_

#include <iostream>

struct Point
{
    int x, y;
};

std::istream& operator>>(std::istream& in, Point& point);
#endif
