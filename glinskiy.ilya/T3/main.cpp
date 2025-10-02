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
    std::cout << "ERROR: No input file was passed\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cout << "ERROR: Input file was invalid\n";
    return 1;
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

  while (!std::cin.eof()) {
    std::string command, argument;
    std::cin >> command;

    if (command == "AREA") {
      std::cin >> argument;

      try {
        int num = std::stoi(argument);

        std::cout << std::accumulate(std::begin(polygons), std::end(polygons),
                                     0.0,
                                     [&](double total, Polygon &poly) {
                                       return total + (poly.points.size() == num
                                                           ? poly.getArea()
                                                           : 0);
                                     })
                  << "\n";
      } catch (...) {
        if (argument == "MEAN") {
          std::cout << std::accumulate(std::begin(polygons), std::end(polygons),
                                       0.0) /
                           polygons.size()
                    << "\n";
        } else if (argument == "EVEN" || argument == "ODD") {
          std::cout << std::accumulate(
                           std::begin(polygons), std::end(polygons), 0.0,
                           [&](double total, Polygon &poly) {
                             return total + (poly.isOdd() == (argument == "ODD")
                                                 ? poly.getArea()
                                                 : 0);
                           })
                    << "\n";
        } else {
          std::cin.setstate(std::ios::failbit);
        }
      }

    } else if (command == "MAX") {
      std::cin >> argument;

      if (argument == "AREA") {
        std::cout << std::max_element(std::begin(polygons), std::end(polygons),
                                      std::less{})
                         ->getArea()
                  << "\n";
      } else if (argument == "VERTEXES") {
        std::cout << std::max_element(std::begin(polygons), std::end(polygons),
                                      [](Polygon &poly1, Polygon &poly2) {
                                        return poly1.points.size() <
                                               poly2.points.size();
                                      })
                         ->getVertexes()
                  << "\n";
      } else {
        std::cin.setstate(std::ios::failbit);
      }

    } else if (command == "MIN") {
      std::cin >> argument;

      if (argument == "AREA") {
        std::cout << std::max_element(std::begin(polygons), std::end(polygons),
                                      std::bind(std::less{},
                                                std::placeholders::_2,
                                                std::placeholders::_1))
                         ->getArea()
                  << "\n";
      } else if (argument == "VERTEXES") {
        std::cout << std::max_element(std::begin(polygons), std::end(polygons),
                                      [](Polygon &poly1, Polygon &poly2) {
                                        return poly1.points.size() >
                                               poly2.points.size();
                                      })
                         ->getVertexes()
                  << "\n";
      } else {
        std::cin.setstate(std::ios::failbit);
      }

    } else if (command == "COUNT") {
      std::cin >> argument;

      try {
        int num = std::stoi(argument);

        std::cout << std::count_if(std::begin(polygons), std::end(polygons),
                                   [&](Polygon &poly) {
                                     return poly.points.size() == num;
                                   })
                  << "\n";
      } catch (...) {
        if (argument == "EVEN" || argument == "ODD") {
          std::cout << std::count_if(std::begin(polygons), std::end(polygons),
                                     [&](Polygon &poly) {
                                       return poly.isOdd() ==
                                              (argument == "ODD");
                                     })
                    << "\n";
        } else {
          std::cin.setstate(std::ios::failbit);
        }
      }

    } else if (command == "LESSAREA") {
      Polygon poly;
      std::cin >> poly;

      if (!std::cin.fail())
        std::cout << std::count_if(
                         std::begin(polygons), std::end(polygons),
                         std::bind(std::less{}, std::placeholders::_1, poly))
                  << "\n";

    } else if (command == "MAXSEQ") {
      Polygon poly;
      std::cin >> poly;

      size_t maxCount = 0;

      if (!std::cin.fail()) {
        std::accumulate(std::begin(polygons), std::end(polygons), 0,
                        [&](size_t count, Polygon &curPoly) {
                          if (curPoly == poly) {
                            if (++count > maxCount)
                              maxCount = count;

                            return count;
                          }

                          return static_cast<size_t>(0);
                        });

        std::cout << maxCount << "\n";
      }

    } else {
      std::cin.setstate(std::ios::failbit);
    }

    if (std::cin.eof())
      break;
    else if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
