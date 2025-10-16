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

#endif
