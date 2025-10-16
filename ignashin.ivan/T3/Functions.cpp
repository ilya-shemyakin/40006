#include "Functions.h"
#include "Polygon.h"
#include "Functors.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <limits>


std::istream& operator>>(std::istream& in, Point& p) {
    char ch;
    if (in >> ch && ch == '(' && in >> p.x >> ch && ch == ';' && in >> p.y >> ch && ch == ')') {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly) {
    poly.points.clear();
    int count;

    if (!(in >> count)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (count < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < count; ++i) {
        char nextChar = in.peek();
        if (nextChar == '\n') {
            in.setstate(std::ios::failbit);
            return in;
        }
        Point p;
        if (!(in >> p)) {
            return in;
        }
        poly.points.push_back(p);
    }

    char nextChar = in.peek();
    if (nextChar != '\n' && nextChar != EOF) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

void invalidCommand() {
    std::cout << "<INVALID COMMAND>\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool is_empty(std::vector<Polygon>& data)
{
    if (data.size() == static_cast<size_t>(0)) {
        return true;
    }
    return false;
}

bool isNumber(std::string& arg) {
    try {
        std::stoi(arg);
        return true;
    }
    catch (...) {
        return false;
    }
}

double areaEvenOdd(std::string& arg, std::vector<Polygon>& data) {
    EvenOddFilter filter((arg == "EVEN") ? true : false);

    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [&filter](double sum, Polygon& figure) {
            if (filter(figure)) {
                return sum + calculateArea(figure.points);
            }
            return sum;
        }
    );
    return output;
}

double areaMean(std::vector<Polygon>& data) {
    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [](double sum, Polygon& figure) {
            return sum + calculateArea(figure.points);
        }
    );

    return output / static_cast<double>(data.size());
}

double areaNum(int arg, std::vector<Polygon>& data) {
    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [arg](double sum, Polygon& figure) {
            if (figure.points.size() == static_cast<size_t>(arg)) {
                return sum + calculateArea(figure.points);
            }
            return sum;
        }
    );

    return output;
}

double calculateArea(std::vector<Point>& points) {
    double area = 0.0;
    size_t n = points.size();

    for (size_t i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += (points[i].x * points[j].y) - (points[i].y * points[j].x);
    }
    return std::abs(area) / 2.0;
}

double maxMinArea(std::string& command, std::vector<Polygon>& data) {
    auto output = (command == "MAX")
        ? std::max_element(data.begin(), data.end(), AreaComparator())
        : std::min_element(data.begin(), data.end(), AreaComparator());
    return calculateArea(output->points);
}

size_t maxMinVertexes(std::string& command, std::vector<Polygon>& data) {
    auto output = (command == "MAX")
        ? std::max_element(data.begin(), data.end(), VertexCountComparator())
        : std::min_element(data.begin(), data.end(), VertexCountComparator());
    return output->points.size();
}

int count(std::string arg, std::vector<Polygon>& data) {
    EvenOddFilter filter((arg == "EVEN") ? true : false);
    int output = std::count_if(data.begin(), data.end(), filter);
    return output;
}

int count(int arg, std::vector<Polygon>& data) {
    int output = std::count_if(
        data.begin(),
        data.end(),
        [arg](Polygon& figure) {
            return figure.points.size() == static_cast<size_t>(arg);
        }
    );
    return output;
}

int perms(Polygon& etalon, std::vector<Polygon>& data) {
    EqualPoints comparator;
    auto output = std::count_if(data.begin(), data.end(),
        [&etalon, &comparator](const Polygon& poly) {
            return poly.points.size() == etalon.points.size() &&
                std::is_permutation(
                    poly.points.begin(), poly.points.end(),
                    etalon.points.begin(),
                    std::bind(
                        std::logical_and<bool>(),
                        std::bind(std::equal_to<int>(),
                            std::bind(&Point::x, std::placeholders::_1),
                            std::bind(&Point::x, std::placeholders::_2)
                        ),
                        std::bind(std::equal_to<int>(),
                            std::bind(&Point::y, std::placeholders::_1),
                            std::bind(&Point::y, std::placeholders::_2)
                        )
                    ));
        });
    return static_cast<int>(output);
}

int rightshapes(std::vector<Polygon>& data) {
    int count = std::count_if(data.begin(), data.end(),
        [](Polygon& figure) {
            return hasRightAngle(figure.points);
        });
    return count;
}


bool hasRightAngle(std::vector<Point>& figure) {
    for (size_t i = 0; i < figure.size(); ++i) {
        const Point& prev = figure[(i + figure.size() - 1) % figure.size()];
        const Point& curr = figure[i];
        const Point& next = figure[(i + 1) % figure.size()];

        int ba_x = prev.x - curr.x;
        int ba_y = prev.y - curr.y;
        int bc_x = next.x - curr.x;
        int bc_y = next.y - curr.y;

        int dot = ba_x * bc_x + ba_y * bc_y;

        if (dot == 0) {
            return true;
        }
    }
    return false;
}
