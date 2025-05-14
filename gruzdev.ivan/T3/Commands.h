#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "Polygon.h"
#include <vector>
#include <functional>
#include <limits>

struct AreaCalculator
{
	double operator() (const Polygon& polygon) const;
};

struct VertexCountChecker
{
	bool operator() (const Polygon& polygon, size_t vertexes) const;
};

struct EvenOddChecker
{
	bool operator() (const Polygon& polygon, bool isEven) const;
};

struct AreaComparator
{
	bool operator() (const Polygon& polygon, double target_area) const;
};

struct IntersectionChecker
{
	bool operator()(const Polygon& polygon, const Polygon& target) const;
};

void process_commands(const std::vector<Polygon>& polygons);
void invalid_command();
bool read_subcommand(std::string& subcommand);
bool read_polygon(Polygon& target);

bool on_segment(const Point& p, const Point& q, const Point& r);
int orientation(const Point& p, const Point& q, const Point& r);
bool segments_intersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2);
bool do_intersect(const Polygon& poly1, const Polygon& poly2);
bool point_inside_polygon(const Point& point, const Polygon& poly);
#endif
