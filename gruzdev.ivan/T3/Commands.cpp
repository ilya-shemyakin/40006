#include "Commands.h"
#include "Polygon.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <functional>
#include <limits>

using namespace std::placeholders;

bool VertexCountChecker::operator()(const Polygon& polygon, size_t vertexes) const
{
    return polygon.points.size() == vertexes;
}

bool EvenOddChecker::operator()(const Polygon& polygon, bool isEven) const
{
    return (polygon.points.size() % 2 == 0) == isEven;
}

bool IntersectionChecker::operator()(const Polygon& polygon, const Polygon& target) const
{
    return do_intersect(polygon, target);
}

void process_commands(const std::vector<Polygon>& polygons)
{
    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "AREA")
        {
            std::string subcommand;
            if (read_subcommand(subcommand))
            {
                area_command(polygons, subcommand);
            }
        }
        else if (cmd == "MAX" || cmd == "MIN")
        {
            std::string subcommand;
            if (read_subcommand(subcommand))
            {
                max_min_command(polygons, cmd, subcommand);
            }
        }
        else if (cmd == "COUNT")
        {
            std::string subcommand;
            if (read_subcommand(subcommand))
            {
                count_command(polygons, subcommand);
            }
        }
        else if (cmd == "LESSAREA" || cmd == "INTERSECTIONS")
        {
            Polygon target;
            if (read_polygon(target))
            {
                if (cmd == "LESSAREA")
                {
                    lessarea_command(polygons, target);
                }
                else
                {
                    intersections_command(polygons, target);
                }
            }
        }
        else
        {
            invalid_command();
        }
    }
}

void area_command(const std::vector<Polygon>& polygons, const std::string& subcommand)
{
    if (subcommand == "EVEN" || subcommand == "ODD")
    {
        bool isEven = (subcommand == "EVEN");
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [isEven](double acc, const Polygon& poly)
            {
                bool condition = (poly.points.size() % 2 == 0);
                return acc + (condition == isEven ? polygon_area(poly) : 0.0);
            });
        std::cout << std::fixed << std::setprecision(1) << sum << "\n";
    }
    else if (subcommand == "MEAN")
    {
        if (polygons.empty())
        {
            invalid_command();
            return;
        }
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& poly)
            {
                return acc + polygon_area(poly);
            });
        std::cout << std::fixed << std::setprecision(1) << sum / polygons.size() << "\n";
    }
    else
    {
        size_t vertexes;
        if (std::istringstream(subcommand) >> vertexes && vertexes >= 3)
        {
            double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [vertexes](double acc, const Polygon& poly)
                {
                    return acc + (poly.points.size() == vertexes ? polygon_area(poly) : 0.0);
                });
            std::cout << std::fixed << std::setprecision(1) << sum << "\n";
        }
        else
        {
            invalid_command();
        }
    }
}

void max_min_command(const std::vector<Polygon>& polygons, const std::string& cmd, const std::string& subcommand)
{
    if (polygons.empty())
    {
        invalid_command();
        return;
    }

    if (subcommand == "AREA")
    {
        auto extremum = (cmd == "MAX") ?
            std::max_element(polygons.begin(), polygons.end(),
                [](const Polygon& poly1, const Polygon& poly2)
                {
                    return polygon_area(poly1) < polygon_area(poly2);
                }) :
            std::min_element(polygons.begin(), polygons.end(),
                [](const Polygon& poly1, const Polygon& poly2)
                {
                    return polygon_area(poly1) < polygon_area(poly2);
                });
                std::cout << std::fixed << std::setprecision(1) << polygon_area(*extremum) << "\n";
    }
    else if (subcommand == "VERTEXES")
    {
        auto extremum = (cmd == "MAX") ?
            std::max_element(polygons.begin(), polygons.end(),
                [](const Polygon& poly1, const Polygon& poly2)
                {
                    return poly1.points.size() < poly2.points.size();
                }) :
            std::min_element(polygons.begin(), polygons.end(),
                [](const Polygon& poly1, const Polygon& poly2)
                {
                    return poly1.points.size() < poly2.points.size();
                });
                std::cout << extremum->points.size() << "\n";
    }
    else
    {
        invalid_command();
    }
}

void count_command(const std::vector<Polygon>& polygons, const std::string& subcommand)
{
    if (subcommand == "EVEN" || subcommand == "ODD")
    {
        bool isEven = (subcommand == "EVEN");
        auto count = std::count_if(polygons.begin(), polygons.end(),
            std::bind(EvenOddChecker(), _1, isEven));
        std::cout << count << "\n";
    }
    else
    {
        size_t vertexes;
        if (std::istringstream(subcommand) >> vertexes && vertexes >= 3)
        {
            auto count = std::count_if(polygons.begin(), polygons.end(),
                std::bind(VertexCountChecker(), _1, vertexes));
            std::cout << count << "\n";
        }
        else
        {
            invalid_command();
        }
    }
}

void lessarea_command(const std::vector<Polygon>& polygons, const Polygon& target)
{
    double target_area = polygon_area(target);
    auto count = std::count_if(polygons.begin(), polygons.end(),
        [target_area](const Polygon& poly)
        {
            return polygon_area(poly) < target_area;
        });
    std::cout << count << "\n";
}

void intersections_command(const std::vector<Polygon>& polygons, const Polygon& target)
{
    auto count = std::count_if(polygons.begin(), polygons.end(),
        std::bind(IntersectionChecker(), _1, std::cref(target)));
    std::cout << count << "\n";
}

void invalid_command()
{
    std::cout << "<INVALID COMMAND>\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool read_subcommand(std::string& subcommand)
{
    if (!(std::cin >> subcommand))
    {
        invalid_command();
        return false;
    }
    return true;
}

bool read_polygon(Polygon& target)
{
    if (!(std::cin >> target))
    {
        invalid_command();
        return false;
    }
    return true;
}

bool on_segment(const Point& p, const Point& q, const Point& r)
{
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}

int orientation(const Point& p, const Point& q, const Point& r)
{
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
    {
        return 0;
    }
    return (val > 0) ? 1 : 2;
}

bool segments_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
    {
        return true;
    }

    if (o1 == 0 && on_segment(p1, p2, q1))
    {
        return true;
    }
    if (o2 == 0 && on_segment(p1, q2, q1))
    {
        return true;
    }
    if (o3 == 0 && on_segment(p2, p1, q2))
    {
        return true;
    }
    if (o4 == 0 && on_segment(p2, q1, q2))
    {
        return true;
    }

    return false;
}

bool do_intersect(const Polygon& poly1, const Polygon& poly2)
{
    const auto& pts1 = poly1.points;
    const auto& pts2 = poly2.points;

    bool segments_cross = std::any_of(pts1.begin(), pts1.end(),
        [&pts1, &pts2](const Point& p1) {
            const Point& q1 = pts1[(&p1 - &pts1[0] + 1) % pts1.size()];
            return std::any_of(pts2.begin(), pts2.end(),
                [&p1, &q1, &pts2](const Point& p2) {
                    const Point& q2 = pts2[(&p2 - &pts2[0] + 1) % pts2.size()];
                    return segments_intersect(p1, q1, p2, q2);
                });
        });

    if (segments_cross)
    {
        return true;
    }

    return point_inside_polygon(pts1[0], poly2) || point_inside_polygon(pts2[0], poly1);
}

bool point_inside_polygon(const Point& point, const Polygon& poly)
{
    const auto& pts = poly.points;

    return std::accumulate(pts.begin(), pts.end(),
        std::make_pair(false, pts.back()),
        [&point](auto acc, const Point& p1) {
            bool inside = acc.first;
            const Point& p2 = acc.second;

            bool intersect = ((p1.y > point.y) != (p2.y > point.y)) &&
                (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x);

            return std::make_pair(intersect ? !inside : inside, p1);
        }).first;
}
