#ifndef GEOM_H_
#define GEOM_H_
#include <iostream>
#include <vector>

struct Point
{
    int x, y;
};
struct Polygon
{
    std::vector<Point> points;
};

#endif
