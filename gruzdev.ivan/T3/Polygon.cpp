#include "Polygon.h"
#include <numeric>
#include <cmath>

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    polygon.points.clear();
    int vertexes = 0;
    if (!(in >> vertexes) || vertexes < 3)
    {
        in.setstate(std::ios::failbit);
    }

    for (int i = 0; i < vertexes; i++)
    {
        char check = in.peek();
        if (check == '\n')
        {
            in.setstate(std::ios::failbit);
            break;
        }
        Point point;
        if (!(in >> point))
        {
            in.setstate(std::ios::failbit);
            break;
        }

        polygon.points.push_back(point);
    }

    char check =  in.peek();
    if (check != '\n' && check != EOF)
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

double polygon_area(const Polygon& polygon)
{
    const auto& pts = polygon.points;
    auto sum = std::inner_product(
        pts.begin(),
        pts.end() - 1,
        pts.begin() + 1,
        0.0,
        std::plus<>(),
        [](const Point& point1, const Point& point2) {
            return static_cast<double>(point1.x) * point2.y
                - static_cast<double>(point2.x) * point1.y;
        }
    );

    sum += static_cast<double>(pts.back().x) * pts.front().y
        - static_cast<double>(pts.front().x) * pts.back().y;
    return std::abs(sum) / 2.0;
}

