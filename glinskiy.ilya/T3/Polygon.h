#ifndef _POLYGON_H
#define _POLYGON_H

#include "IO.h"
#include <iostream>
#include <vector>

struct Point {
  int x, y;
};

struct Polygon {
  std::vector<Point> points;

  bool isOdd() const;
  size_t getVertexes() const;
  double getArea() const;
};

std::istream &operator>>(std::istream &in, Polygon &poly);
std::ostream &operator<<(std::ostream &out, const Polygon &poly);
bool operator==(const Polygon &poly1, const Polygon &poly2);
bool operator!=(const Polygon &poly1, const Polygon &poly2);
bool operator<(const Polygon &poly1, const Polygon &poly2);
int operator+(const int num, const Polygon &poly);

bool operator==(const Point &point1, const Point &point2);
std::ostream &operator<<(std::ostream &out, const Point &point);

#endif
