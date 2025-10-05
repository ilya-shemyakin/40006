#include "Functions.h"
#include "Geometry.h"
#include <limits>
#include <sstream>
#include <iostream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iomanip>

double computeAreaEvenOdd(const std::vector<Polygon>& polygons, Parity parity) {
  AddFilteredArea adder(parity);
  return std::accumulate(
    polygons.begin(), polygons.end(), 0.0,
    std::bind(&AddFilteredArea::apply, adder,
      std::placeholders::_1, std::placeholders::_2));
}

double computeAreaMean(const std::vector<Polygon>& polygons) {
  double total = std::accumulate(
    polygons.begin(), polygons.end(), 0.0,
    std::bind(&AddArea, std::placeholders::_1, std::placeholders::_2)
  );
  return total / polygons.size();
}

double computeAreaByVertexCount(const std::vector<Polygon>& polygons, int n) {
  AddSpecificVertexCountArea adder(n);
  return std::accumulate(
    polygons.begin(), polygons.end(), 0.0,
    std::bind(&AddSpecificVertexCountArea::apply, adder,
      std::placeholders::_1, std::placeholders::_2));
}

double computeExtremumArea(const std::vector<Polygon>& polygons, bool isMax) {
  auto cmpArea = [](const Polygon& a, const Polygon& b) {
    return calculateArea(a.points_) < calculateArea(b.points_);
    };

  auto it = isMax
    ? std::max_element(polygons.begin(), polygons.end(), cmpArea)
    : std::min_element(polygons.begin(), polygons.end(), cmpArea);
  return calculateArea(it->points_);
}

int computeExtremumVertexes(const std::vector<Polygon>& polygons, bool isMax) {
  auto cmpVertexes = [](const Polygon& a, const Polygon& b) {
    return a.points_.size() < b.points_.size();
    };

  auto it = isMax
    ? std::max_element(polygons.begin(), polygons.end(), cmpVertexes)
    : std::min_element(polygons.begin(), polygons.end(), cmpVertexes);
  return it->points_.size();
}

int computeCountEvenOdd(const std::vector<Polygon>& polygons, Parity parity) {
  return std::count_if(polygons.begin(), polygons.end(),
    [parity](const Polygon& p) {
      int size = static_cast<int>(p.points_.size());
      if (parity == Parity::EVEN) {
        return size % 2 == 0;
      }
      else {
        return size % 2 == 1;
      }
    });
}

int computeCountByVertexCount(const std::vector<Polygon>& polygons, int n) {
  return std::count_if(polygons.begin(), polygons.end(),
    [n](const Polygon& p) {
      return static_cast<int>(p.points_.size()) == n;
    });
}

int computeEcho(std::vector<Polygon>& polygons, const Polygon& target) {
  std::vector<size_t> indices;
  std::transform(polygons.begin(), polygons.end(), std::back_inserter(indices),
    [&](const Polygon& p) {
      return (p == target) ? (&p - &polygons[0]) : std::numeric_limits<size_t>::max();
    });

  indices.erase(std::remove(indices.begin(), indices.end(), std::numeric_limits<size_t>::max()), indices.end());

  std::for_each(indices.rbegin(), indices.rend(), [&](size_t idx) {
    polygons.insert(polygons.begin() + idx + 1, target);
    });

  return static_cast<int>(indices.size());
}


int countRectangles(const std::vector<Polygon>& polygons) {
  return static_cast<int>(std::count_if(polygons.begin(), polygons.end(), isRectangle));
}

int countIntersections(const std::vector<Polygon>& polygons, const Polygon& target) {
  return static_cast<int>(std::count_if(polygons.begin(), polygons.end(),
    [&target](const Polygon& poly) {
      return doPolygonsIntersect(poly, target);
    }));
}

bool hasMoreArguments(std::istream& in) {
  std::string leftover;
  return static_cast<bool>(in >> leftover);
}

void invalidCommand() {
  std::cout << "<INVALID COMMAND>\n";