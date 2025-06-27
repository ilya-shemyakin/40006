#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <vector>
#include <sstream>
#include "Polygon.h"


std::istream& operator>>(std::istream& in, Point& p);
std::istream& operator>>(std::istream& in, Polygon& poly);

void invalidCommand();
bool isEmpty(std::vector<Polygon>& data);

bool isNumber(std::string& arg);
double calculateArea(std::vector<Point>& points);
double areaEvenOdd(std::string& arg, std::vector<Polygon>& data);
double areaMean(std::vector<Polygon>& data);
double areaNum(int arg, std::vector<Polygon>& data);
double maxMinArea(std::string& command, std::vector<Polygon>& data);
size_t maxMinVertexes(std::string& command, std::vector<Polygon>& data);
int count(std::string arg, std::vector<Polygon>& data);
int count(int arg, std::vector<Polygon>& data);
bool hasRightAngle(std::vector<Point>& figure);
int rightShapes(std::vector<Polygon>& data);
bool inFrame(Polygon& poly, std::vector<Polygon>& polygons);

#endif
