#ifndef POLYGON_PROCESSOR_H
#define POLYGON_PROCESSOR_H

#include <vector>
#include <iostream>
#include <functional>

struct Vertex {
    int x, y;
    bool operator==(const Vertex& other) const;
};

struct Shape {
    std::vector<Vertex> vertices;
    bool operator==(const Shape& other) const;
    bool operator<(const Shape& other) const;
};

std::istream& operator>>(std::istream& in, Vertex& v);
std::istream& operator>>(std::istream& in, Shape& shape);

double computeShapeArea(const Shape& shape);
double calculatePolygonArea(const std::vector<Vertex>& vertices);

enum class VertexParity {
    EVEN,
    ODD
};

struct AreaAccumulator {
    double operator()(double sum, const Shape& shape) const;
};

struct FilteredAreaAccumulator {
    VertexParity parity;
    double operator()(double sum, const Shape& shape) const;
};

struct VertexCountAreaAccumulator {
    int count;
    double operator()(double sum, const Shape& shape) const;
};

#endif
