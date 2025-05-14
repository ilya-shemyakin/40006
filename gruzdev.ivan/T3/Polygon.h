#ifndef POLYGON_H_
#define POLYGON_H_

#include "Point.h"
#include <vector>

struct Polygon
{
	std::vector<Point> points;
};

std::istream& operator>>(std::istream& in, Polygon& polygon);
double polygon_area(const Polygon& polygon);
#endif
