#ifndef GEOMETRY_H_
#define GEOMETRY_H_
#include <vector>
#include <string>
#include <cstdint>

struct Point {
  int x_, y_;

  bool operator==(const Point& other) const;
};

struct Polygon {
  std::vector<Point> points_;

  bool operator<(const Polygon& other) const;
  bool operator==(const Polygon& other) const;
};

std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);

double calculateArea(const std::vector<Point>& points);

enum class Parity : uint8_t
{
  EVEN,
  ODD
};

struct EvenOddFilter {
  int mod_;
  EvenOddFilter(const std::string& arg) : mod_(arg == "EVEN" ? 0 : 1) {}

  bool operator()(const Polygon& figure) const;
};

struct AddFilteredArea {
  Parity parity_;

  AddFilteredArea(Parity parity) : parity_(parity) {}

  double apply(double acc, const Polygon& p) const;
};

struct AddSpecificVertexCountArea {
  int n_;
  AddSpecificVertexCountArea(int n) : n_(n) {}

  double apply(double acc, const Polygon& p) const;
};

double AddArea(double acc, const Polygon& p);

bool isRectangle(const Polygon& poly);
bool doPolygonsIntersect(const Polygon& poly1, const Polygon& poly2);
bool isPointInPolygon(const Point& point, const Polygon& poly);
bool doSegmentsIntersect(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
int orientation(const Point& p, const Point& q, const Point& r);
bool onSegment(const Point& p, const Point& q, const Point& r);

#endif
