#include "Functions.h"
#include "Polygon.h"
#include "Functors.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <limits>

std::istream& operator>>(std::istream& in, Point& p)
{
    char ch1, ch2, ch3;
    if (in >> ch1 >> p.x >> ch2 >> p.y >> ch3 && ch1 == '(' && ch2 == ';' && ch3 == ')')
    {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly)
{
    poly.points.clear();
    int count;
    if (!(in >> count) || count < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < count; ++i)
    {
        if (in.peek() == '\n')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        Point p;
        if (!(in >> p))
        {
            return in;
        }
        poly.points.push_back(p);
    }

    if (in.peek() != '\n' && in.peek() != EOF)
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

void invalidCommand()
{
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isEmpty(std::vector<Polygon>& data)
{
    return data.empty();
}

bool isNumber(std::string& arg)
{
    try
    {
        std::stoi(arg);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

double calculateArea(std::vector<Point>& points)
{
    double area = 0.0;
    size_t n = points.size();
    for (size_t i = 0; i < n; ++i)
    {
        int j = (i + 1) % n;
        area += (points[i].x * points[j].y) - (points[i].y * points[j].x);
    }
    return std::abs(area) / 2.0;
}

double areaEvenOdd(std::string& arg, std::vector<Polygon>& data)
{
    bool isEven = (arg == "EVEN");
    return std::accumulate(data.begin(), data.end(), 0.0,
        [isEven](double sum, Polygon& poly)
        {
            bool condition = (poly.points.size() % 2 == 0);
            return (isEven == condition) ? sum + calculateArea(poly.points) : sum;
        });
}

double areaMean(std::vector<Polygon>& data)
{
    if (data.empty())
    {
        return 0.0;
    }
    double total = std::accumulate(data.begin(), data.end(), 0.0,
        [](double sum, Polygon& poly)
        {
            return sum + calculateArea(poly.points);
        });
    return total / data.size();
}

double areaNum(int arg, std::vector<Polygon>& data)
{
    double output = std::accumulate(
        data.begin(),
        data.end(),
        0.0,
        [arg](double sum, Polygon& figure)
        {
            if (figure.points.size() == static_cast<size_t>(arg)) 
            {
                return sum + calculateArea(figure.points);
            }
            return sum;
        }
    );

    return output;
}
double maxMinArea(std::string& command, std::vector<Polygon>& data)
{
    auto output = (command == "MAX")
        ? std::max_element(data.begin(), data.end(), AreaComparator())
        : std::min_element(data.begin(), data.end(), AreaComparator());
    return calculateArea(output->points);
}

size_t maxMinVertexes(std::string& command, std::vector<Polygon>& data)
{
    auto output = (command == "MAX")
        ? std::max_element(data.begin(), data.end(), VertexCountComparator())
        : std::min_element(data.begin(), data.end(), VertexCountComparator());
    return output->points.size();
}

int count(std::string arg, std::vector<Polygon>& data)
{
    bool isEven = (arg == "EVEN");
    return std::count_if(data.begin(), data.end(),
        [isEven](Polygon& poly)
        {
            return (poly.points.size() % 2 == 0) == isEven;
        });
}

int count(int arg, std::vector<Polygon>& data)
{
    if (arg < 3) return 0;
    return std::count_if(data.begin(), data.end(),
        [arg](Polygon& poly)
        {
            return poly.points.size() == static_cast<size_t>(arg);
        });
}

bool hasRightAngle(std::vector<Point>& figure)
{
    for (size_t i = 0; i < figure.size(); ++i)
    {
        const Point& prev = figure[(i + figure.size() - 1) % figure.size()];
        const Point& curr = figure[i];
        const Point& next = figure[(i + 1) % figure.size()];
        int ba_x = prev.x - curr.x;
        int ba_y = prev.y - curr.y;
        int bc_x = next.x - curr.x;
        int bc_y = next.y - curr.y;
        int dot = ba_x * bc_x + ba_y * bc_y;
        if (dot == 0)
        {
            return true;
        }
    }
    return false;
}

int rightShapes(std::vector<Polygon>& data)
{
    return std::count_if(data.begin(), data.end(),
        [](Polygon& poly)
        {
            return hasRightAngle(poly.points);
        });
}

bool inFrame(Polygon& poly, std::vector<Polygon>& polygons)
{
    if (polygons.empty() || poly.points.empty())
    {
        return false;
    }
    int minX = polygons[0].points[0].x;
    int maxX = polygons[0].points[0].x;
    int minY = polygons[0].points[0].y;
    int maxY = polygons[0].points[0].y;
    for (const auto& p : polygons)
    {
        for (const auto& point : p.points)
        {
            minX = std::min(minX, point.x);
            maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y);
            maxY = std::max(maxY, point.y);
        }
    }
    for (const auto& point : poly.points)
    {
        if (point.x < minX || point.x > maxX || point.y < minY || point.y > maxY) return false;
    }
    return true;
}
