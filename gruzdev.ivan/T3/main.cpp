#include<iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <map>
#include <functional>
#include <limits>
#include <stdexcept>

struct Point
{
    int x, y;
};

struct Polygon
{
    std::vector<Point> points;
};

std::istream& operator>>(std::istream& in, Point& point);
std::istream& operator>>(std::istream& in, Polygon& polygon);
double polygon_area(const Polygon& polygon);
void process_commands(const std::vector<Polygon>& polygons);
bool segment(const Point& p1, const Point& p2, const Point& f1, const Point& f2);
bool do_intersect(const Polygon& poly1, const Polygon& poly2);
void invalid_command();
bool read_subcommand(std::string& subcommand);
bool read_polygon(Polygon& target);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << argv[0] << "\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cout << "Cannot open file: " << argv[1] << "\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    std::copy(
        std::istream_iterator<Polygon>(inputFile),
        std::istream_iterator<Polygon>(),
        std::back_inserter(polygons)
    );

    process_commands(polygons);
    return 0;
}

std::istream& operator>>(std::istream& in, Point& point)
{
    char ch1, ch2, ch3;
    in >> ch1 >> point.x >> ch2 >> point.y >> ch3;
    if (ch1 != '(' || ch2 != ';' || ch3 != ')')
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}


std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    int vertexes;
    if (!(in >> vertexes))
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    polygon.points.clear();
    polygon.points.reserve(vertexes);

    for (int i = 0; i < vertexes; ++i)
    {
        Point point;
        if (!(in >> point))
        {
            in.setstate(std::ios::failbit);
            break;
        }
        polygon.points.push_back(point);
    }
    return in;
}


double polygon_area(const Polygon& polygon)
{
    if (polygon.points.size() < 3)
    {
        return 0;
    }

    const auto& pts = polygon.points;
    auto sum = std::inner_product(
        pts.begin(),
        pts.end() - 1,
        pts.begin() + 1,
        0.0,
        std::plus<>(),
        [](const Point& point1, const Point& point2)
        {
            return static_cast<double>(point1.x) * point2.y - static_cast<double>(point2.x) * point1.y;
        }
    );

    sum += static_cast<double>(pts.back().x) * pts.front().y - static_cast<double>(pts.front().x) * pts.back().y;
    return std::abs(sum) / 2.0;
}

void process_commands(const std::vector<Polygon>& polygons)
{
    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "AREA")
        {
            std::string subcommand;
            if (!read_subcommand(subcommand))
            {
                continue;
            }

            if (subcommand == "EVEN" || subcommand == "ODD")
            {
                bool isEven = (subcommand == "EVEN");
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [isEven](double acc, const Polygon& polygon)
                    {
                        bool condition = (polygon.points.size() % 2 == 0);
                        return acc + (condition == isEven ? polygon_area(polygon) : 0.0);
                    });
                std::cout << std::fixed << std::setprecision(1) << sum << "\n";
            }

            else if (subcommand == "MEAN")
            {
                if (polygons.empty())
                {
                    invalid_command();
                    continue;
                }

                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [](double acc, const Polygon& polygon)
                    {
                        return acc + polygon_area(polygon);
                    });
                std::cout << std::fixed << std::setprecision(1) << (sum / polygons.size()) << "\n";
            }

            else
            {
                size_t vertexes;
                if (std::istringstream(subcommand) >> vertexes)
                {
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [vertexes](double acc, const Polygon& polygon)
                        {
                            return acc + (polygon.points.size() == vertexes ? polygon_area(polygon) : 0.0);
                        });
                    std::cout << std::fixed << std::setprecision(1) << sum << "\n";
                }
                else
                {
                    invalid_command();
                }
            }
        }

        else if (cmd == "MAX" || cmd == "MIN")
        {
            std::string subcommand;
            if (!read_subcommand(subcommand))
            {
                continue;
            }

            if (polygons.empty())
            {
                invalid_command();
                continue;
            }

            if (subcommand == "AREA")
            {
                auto extremum = (cmd == "MAX") ?
                    std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& poly1, const Polygon& poly2)
                        {
                            return polygon_area(poly1) < polygon_area(poly2);
                        }):
                    std::min_element(polygons.begin(), polygons.end(),
                        [](const Polygon& poly1, const Polygon& poly2) {
                            return polygon_area(poly1) < polygon_area(poly2);
                        });
                std::cout << std::fixed << std::setprecision(1) << polygon_area(*extremum) << "\n";

            }
            else if (subcommand == "VERTEXES")
            {
                auto extremum = (cmd =="MAX")?
                    std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& poly1, const Polygon& poly2)
                        {
                            return poly1.points.size() < poly2.points.size();
                        }):
                    std::min_element(polygons.begin(), polygons.end(),
                        [](const Polygon& poly1, const Polygon& poly2) {
                            return poly1.points.size() < poly2.points.size();
                        });
                        std::cout << extremum->points.size() << "\n";
            }
            else
            {
                invalid_command();
            }
        }
        else if (cmd == "COUNT")
        {
            std::string subcommand;
            if (!read_subcommand(subcommand))
            {
                continue;
            }

            if (subcommand == "EVEN" || subcommand == "ODD")
            {
                bool isEven = (subcommand == "EVEN");
                auto counter = std::count_if(polygons.begin(), polygons.end(),
                    [isEven](const Polygon& polygon) {
                        return (polygon.points.size() % 2 == 0) == isEven;
                    });
                std::cout << counter << "\n";
            }
            else
            {
                size_t vertexes;
                if (std::istringstream(subcommand) >> vertexes)
                {
                    auto counter = std::count_if(polygons.begin(), polygons.end(),
                        [vertexes](const Polygon& polygon) {
                            return polygon.points.size() == vertexes;
                        });
                    std::cout << counter << "\n";
                }
                else
                {
                    invalid_command();
                }
            }
        }
        else if (cmd == "LESSAREA" || cmd == "INTERSECTIONS")
        {
            Polygon target;
            if (!read_polygon(target))
            {
                continue;
            }

            if (cmd == "LESSAREA")
            {
                double target_area = polygon_area(target);
                auto counter = std::count_if(polygons.begin(), polygons.end(),
                    [target_area](const Polygon& polygon) {
                        return polygon_area(polygon) < target_area;
                    });
                std::cout << counter << "\n";
            }
            else
            {
                auto counter = std::count_if(polygons.begin(), polygons.end(),
                    [&target](const Polygon& polygon) {
                        return do_intersect(polygon, target);
                    });
                std::cout << counter << "\n";
            }
        }
        else
        {
            invalid_command();
        }

    }
}

bool segment(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
    if (std::max(p1.x, p2.x) < std::min(q1.x, q2.x) ||
        std::max(q1.x, q2.x) < std::min(p1.x, p2.x) ||
        std::max(p1.y, p2.y) < std::min(q1.y, q2.y) ||
        std::max(q1.y, q2.y) < std::min(p1.y, p2.y)){
        return false;
    }

    auto cross = [](const Point& a, const Point& b, const Point& c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        };

    return (cross(p1, p2, q1) * cross(p1, p2, q2) <= 0) &&
        (cross(q1, q2, p1) * cross(q1, q2, p2) <= 0);
}

bool do_intersect(const Polygon& poly1, const Polygon& poly2)
{
    if (&poly1 == &poly2)
    {
        return false;
    }

    for (size_t i = 0; i < poly1.points.size(); ++i)
    {
        const Point& p1 = poly1.points[i];
        const Point& p2 = poly1.points[(i + 1) % poly1.points.size()];

        for (size_t j = 0; j < poly2.points.size(); ++j)
        {
            const Point& q1 = poly2.points[j];
            const Point& q2 = poly2.points[(j + 1) % poly2.points.size()];

            if (segment(p1, p2, q1, q2))
            {
                return true;
            }
        }
    }
    return false;
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
