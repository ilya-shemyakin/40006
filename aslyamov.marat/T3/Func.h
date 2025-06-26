#pragma once
#ifndef FUNC_H_
#define FUNC_H_
#include <iostream>
#include "Geom.h"


std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& pgln);
void invalid_command();
double area(std::string& arg, const std::vector<Polygon>& data);
bool is_num(std::string& str);
double calculate_area(const std::vector<Point>& points);
double max_min_area(std::string& arg, const std::vector<Polygon>& data);
size_t max_min_verts(std::string& arg, const std::vector<Polygon>& data);
size_t count(std::string& arg, const std::vector<Polygon>& data);
size_t less_area(const Polygon& figure, const std::vector<Polygon>& data);
Polygon normalize_polygon(const Polygon& plgn);
size_t same(const Polygon& figure, const std::vector<Polygon>& data);
void print_data(const std::vector<Polygon>& data);

#endif
