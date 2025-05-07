#include <algorithm>
#include <vector>

struct Point {
  int x, y;

  bool operator==(Point &other) { return x == other.x && y == other.y; }
};

struct Polygon {
  std::vector<Point> points;

  double area() {
    if (points.size() < 3)
      return 0.0;

    double a = 0.0;

    for (size_t i = 0; i < points.size(); ++i) {
      const Point &p1 = points[i];
      const Point &p2 = points[(i + 1) % points.size()];
      a += (p1.x * p2.y - p2.x * p1.y);
    }

    return std::abs(a) / 2.0;
  }

  bool operator==(Polygon &other) { return points == other.points; }
};

int main() {}
