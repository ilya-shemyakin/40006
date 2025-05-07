#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Point {
  int x, y;

  // bool operator==(const Point &other) { return x == other.x && y == other.y;
  // }
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

  // bool operator==(const Polygon &other) { return points == other.points; }
};

std::vector<Polygon> readPolygons(const std::string &filename) {
  std::ifstream in(filename);
  std::vector<Polygon> result;
  std::string line;

  while (std::getline(in, line)) {
    std::istringstream iss(line);
    int pointCount;

    if (!(iss >> pointCount) || pointCount <= 0)
      continue;

    std::vector<Point> points;

    for (int i = 0; i < pointCount; ++i) {
      char ch1, ch2, ch3;
      int x, y;
      iss >> ch1 >> x >> ch2 >> y >> ch3;

      if (!iss || ch1 != '(' || ch2 != ';' || ch3 != ')') {
        points.clear();
        break;
      }

      points.push_back({x, y});
    }

    if (points.size() == pointCount) {
      result.push_back({points});
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    throw "Missing filename\n";
  }

  std::vector<Polygon> polygons = readPolygons(argv[1]);
}
