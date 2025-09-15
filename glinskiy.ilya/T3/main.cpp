#include "IO.h"
#include "Polygon.h"
#include <algorithm>
#include <exception>
#include <fstream>
#include <functional>
#include <iterator>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

// VARIANT 2
// LESSAREA MAXSEQ

int main(int argc, char *argv[]) {
  if (argc < 2) {
    throw std::invalid_argument("No input file was passed");
  }

  std::ifstream file(argv[1]);
  if (!file) {
    throw std::invalid_argument("Input file was invalid");
  }

  std::vector<Polygon> polygons;

  while (!file.eof()) {
    std::copy(std::istream_iterator<Polygon>(file),
              std::istream_iterator<Polygon>(), std::back_inserter(polygons));

    if (file.fail()) {
      file.clear();
      file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::copy(std::begin(polygons), std::end(polygons),
            std::ostream_iterator<Polygon>(std::cout, "\n"));
  std::cout << "\n";

  while (!std::cin.eof()) {
    std::string command, argument;
    double result;

    std::cin >> command;

    if (command == "AREA") {
      std::cin >> argument;

      try {
        int num = std::stoi(argument);

        result = std::accumulate(
            std::begin(polygons), std::end(polygons), 0.0,
            [&](int total, const Polygon &poly) {
              return total + (poly.points.size() == num ? poly.getArea() : 0);
            });
      } catch (...) {
        if (argument == "MEAN") {
          result =
              std::accumulate(std::begin(polygons), std::end(polygons), 0.0) /
              polygons.size();
        } else if (argument == "EVEN" || argument == "ODD") {
          result = std::accumulate(std::begin(polygons), std::end(polygons),
                                   0.0, [&](int total, const Polygon &poly) {
                                     return total +
                                            (poly.isOdd() == (argument == "ODD")
                                                 ? poly.getArea()
                                                 : 0);
                                   });
        } else {
          std::cin.setstate(std::ios::failbit);
        }
      }

    } else if (command == "MAX") {
      std::cin >> argument;

      if (argument == "AREA") {
        result = (*std::max_element(std::begin(polygons), std::end(polygons),
                                    std::less{}))
                     .getArea();
      } else if (argument == "VERTEXES") {
        result = (*std::max_element(std::begin(polygons), std::end(polygons),
                                    [](Polygon &poly1, Polygon &poly2) {
                                      return poly1.points.size() <
                                             poly2.points.size();
                                    }))
                     .getVertexes();
      } else {
        std::cin.setstate(std::ios::failbit);
      }

    } else if (command == "MIN") {
      std::cin >> argument;

      if (argument == "AREA") {
        result =
            (*std::max_element(std::begin(polygons), std::end(polygons),
                               std::bind(std::less{}, std::placeholders::_2,
                                         std::placeholders::_1)))
                .getArea();
      } else if (argument == "VERTEXES") {
        result = (*std::max_element(std::begin(polygons), std::end(polygons),
                                    [](Polygon &poly1, Polygon &poly2) {
                                      return poly1.points.size() >
                                             poly2.points.size();
                                    }))
                     .getVertexes();
      } else {
        std::cin.setstate(std::ios::failbit);
      }

    } else if (command == "COUNT") {
      std::cin >> argument;

      try {
        int num = std::stoi(argument);

        result = std::count_if(
            std::begin(polygons), std::end(polygons),
            [&](Polygon poly) { return poly.points.size() == num; });
      } catch (...) {
        if (argument == "EVEN" || argument == "ODD") {
          result = std::count_if(std::begin(polygons), std::end(polygons),
                                 [&](Polygon poly) {
                                   return poly.isOdd() == (argument == "ODD");
                                 });
        } else {
          std::cin.setstate(std::ios::failbit);
        }
      }

    } else if (command == "LESSAREA") {
      Polygon poly;
      std::cin >> poly;

      if (!std::cin.fail())
        result =
            std::count_if(std::begin(polygons), std::end(polygons),
                          std::bind(std::less{}, std::placeholders::_1, poly));

    } else if (command == "MAXSEQ") {
      size_t maxCount = 1;
      Polygon prevPoly;

      result = std::accumulate(std::begin(polygons), std::end(polygons), 1,
                               [&](size_t count, Polygon &curPoly) {
                                 if (curPoly == prevPoly) {
                                   if (++count > maxCount)
                                     maxCount = count;

                                   prevPoly = curPoly;
                                   return count;
                                 } else {
                                   prevPoly = curPoly;
                                   return static_cast<size_t>(1);
                                 }
                               });

    } else {
      std::cin.setstate(std::ios::failbit);
    }

    if (std::cin.eof())
      break;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    } else {
      std::cout << result << "\n";
    }
  }

  return 0;
}