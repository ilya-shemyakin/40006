#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <limits>
#include <string>
#include "Func.h"
#include "Functors.h"


std::istream& operator>>(std::istream& in, Polygon& plgn) {
    plgn.points.clear();
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    int points_count;
    in >> points_count;
    if (points_count < 3 || !in) {
        in.setstate(std::ios::failbit);
        return in;
    }
    for (int i = 0; i < points_count; i++) {
        Point p;
        in >> p;
        if (!in) {
            return in;
        }
        plgn.points.push_back(p);
    }
    return in;
}

std::istream& operator>>(std::istream& in, Point& p) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c;
    if (in >> c && c == '(' && in >> p.x &&
        in >> c && c == ';' && in >> p.y &&
        in >> c && c == ')') {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

bool operator==(const Point& a, const Point& b) {
    return (a.x == b.x && a.y == b.y);
}

void invalid_command() {
    std::cout << "\t<INVALID COMMAND>\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double area(std::string& arg, const std::vector<Polygon>& data) {
    double out = -1;
    bool isEven = (arg == "EVEN");
    if (arg == "EVEN" || arg == "ODD") {
        out = std::accumulate(data.begin(), data.end(), 0.0,
            [isEven](double sum, const Polygon& figure) {
                if ((figure.points.size() % 2 == 0) == isEven) {
                    return sum + calculate_area(figure.points);
                }
                return sum;
            });
        return out;
    }
    else if (arg == "MEAN") {
        if (data.size() == 0) {
            return -1;
        }
        out = std::accumulate(data.begin(), data.end(), 0.0,
            [](double sum, const Polygon& fig) {
                return sum + calculate_area(fig.points);
            });
        return out / data.size();
    }
    else {
        size_t verts = std::stoi(arg);
        out = std::accumulate(data.begin(), data.end(), 0.0,
            [verts](double sum, const Polygon& figure) {
                if (figure.points.size() == verts) {
                    return sum + calculate_area(figure.points);
                }
                return sum;
            });
        return out;
    }
}

double calculate_area(const std::vector<Point>& points) {
    double result = 0;
    size_t n = points.size();
    for (size_t i = 0; i < n; i++) {
        result += (points[i].x * points[(i + 1) % n].y - points[i].y * points[(i + 1) % n].x);
    }
    return std::abs(result) / 2;
}

bool is_num(std::string& str) {
    try {
        std::stoi(str);
        return 1;
    }
    catch (...) {
        return 0;
    }
}

double max_min_area(std::string& arg, const std::vector<Polygon>& data) {
    if (arg == "MAX") {
        auto ma = std::max_element(data.begin(), data.end(), AreaComp());
        return calculate_area(ma->points);
    }
    else {
        auto ma = std::min_element(data.begin(), data.end(), AreaComp());
        return calculate_area(ma->points);
    }
}

size_t max_min_verts(std::string& arg, const std::vector<Polygon>& data) {
    if (arg == "MAX") {
        auto mv = std::max_element(data.begin(), data.end(), VertsComp());
        return mv->points.size();
    }
    else {
        auto mv = std::min_element(data.begin(), data.end(), VertsComp());
        return mv->points.size();
    }
}

size_t count(std::string& arg, const std::vector<Polygon>& data) {
    if (arg == "EVEN") {
        return std::count_if(data.begin(), data.end(), IsEven());
    }
    else if (arg == "ODD") {
        return std::count_if(data.begin(), data.end(), IsOdd());
    }
    else {
        size_t verts = std::stoi(arg);
        return std::count_if(data.begin(), data.end(), VertexCount(verts));
    }
}

size_t less_area(const Polygon& figure, const std::vector<Polygon>& data) {
    auto la = std::bind(AreaComp{}, std::placeholders::_1, figure);
    return std::count_if(data.begin(), data.end(), la);
}

Polygon normalize_polygon(const Polygon& plgn) {
    Polygon result;
    Point base = plgn.points[0];
    for (const Point& p : plgn.points) {
        result.points.push_back({p.x - base.x, p.y - base.y});
    }
    std::sort(result.points.begin(), result.points.end(), [](const Point& a, const Point& b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
        });
    return result;
}

size_t same(const Polygon& figure, const std::vector<Polygon>& data) {
    Polygon target_figure = normalize_polygon(figure);
    return std::count_if(data.begin(), data.end(), [&](const Polygon& figure) {
            if (figure.points.size() != target_figure.points.size()) {
                return false;
            }
            return normalize_polygon(figure).points == target_figure.points;
        });
}
