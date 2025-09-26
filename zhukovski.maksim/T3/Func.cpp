#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <limits>
#include <string>
#include "Func.h"

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
    std::cout << "<INVALID COMMAND>\n";
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

Polygon normalize_polygon(const Polygon& plgn) {
    if (plgn.points.empty()) {
        return plgn;
    }
    Polygon result = plgn;
    auto& points = result.points;
    auto comp = [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
        };
    auto min_it = std::min_element(points.begin(), points.end(), comp);
    std::rotate(points.begin(), min_it, points.end());
    Point base = points[0];
    std::transform(points.begin(), points.end(), points.begin(),
        [&base](Point p) { return Point{ p.x - base.x, p.y - base.y }; });
    double signed_area = 0.0;
    size_t n = points.size();
    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        signed_area += static_cast<double>(points[i].x) * points[j].y -
            static_cast<double>(points[i].y) * points[j].x;
    }
    if (signed_area < 0) {
        std::reverse(points.begin() + 1, points.end());
    }
    return result;
}

size_t same(const Polygon& plgn, const std::vector<Polygon>& data) {
    Polygon target_figure = normalize_polygon(plgn);
    return std::count_if(data.begin(), data.end(), [&](const Polygon& figure) {
        if (figure.points.size() != target_figure.points.size()) {
            return false;
        }
        return normalize_polygon(figure).points == target_figure.points;
        });
}

size_t rmecho(const Polygon& plgn, std::vector<Polygon>& data) {
    Polygon target = normalize_polygon(plgn);
    auto pred = [&](const Polygon& a, const Polygon& b) {
        if (a.points.size() != b.points.size()) return false;
        Polygon na = normalize_polygon(a);
        Polygon nb = normalize_polygon(b);
        return na.points == target.points && nb.points == target.points;
        };
    auto new_end = std::unique(data.begin(), data.end(), pred);
    size_t removed = std::distance(new_end, data.end());
    data.erase(new_end, data.end());
    return removed;
}
