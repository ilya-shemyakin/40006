#include "Polygon.h"
#include "IO.h"
#include <algorithm>
#include <iostream>
#include <iterator>

std::ostream &operator<<(std::ostream &out, const Point &point) {
  out << "(" << point.x << ", " << point.y << ")";
  return out;
}

bool operator==(const Point &point1, const Point &point2) {
  return point1.x == point2.x && point1.y == point2.y;
};

std::istream &operator>>(std::istream &in, Polygon &poly) {
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  int amount;
  Polygon input;

  try {
    in >> amount;

    for (int i = 0; i < amount; i++) {
      Point point;

      in >> CharIO{'('} >> point.x >> CharIO{';'} >> point.y >> CharIO{')'};
      input.points.push_back(point);
    }
  } catch (...) {
    in.setstate(std::ios::failbit);
  }

  if (in)
    poly = input;

  return in;
}

std::ostream &operator<<(std::ostream &out, const Polygon &poly) {
  std::copy(std::begin(poly.points), std::end(poly.points),
            std::ostream_iterator<Point>(out, "   "));

  return out;
}

bool operator==(const Polygon &poly1, const Polygon &poly2) {
  return poly1.points == poly2.points;
}

bool operator!=(const Polygon &poly1, const Polygon &poly2) {
  return !(poly1 == poly2);
}

bool operator<(const Polygon &poly1, const Polygon &poly2) {
  return poly1.getArea() < poly2.getArea();
}

int operator+(const int num, const Polygon &poly) {
  return num + poly.getArea();
};

bool Polygon::isOdd() const { return getVertexes() % 2 == 1; };

size_t Polygon::getVertexes() const { return points.size(); }

double Polygon::getArea() const {
  if (getVertexes() < 3)
    return 0;

  double a = 0;

  for (size_t i = 0; i < getVertexes(); i++) {
    const Point &p1 = points[i];
    const Point &p2 = points[(i + 1) % points.size()];
    a += (p1.x * p2.y - p2.x * p1.y);
  }

  return std::abs(a) / 2.0;
}