#include "PolygonProcessor.h"
#include <cmath>
#include <algorithm>
#include <numeric>

bool Vertex::operator==(const Vertex& other) const {
    return x == other.x && y == other.y;
}

bool Shape::operator==(const Shape& other) const {
    if (vertices.size() != other.vertices.size()) return false;
    return std::equal(vertices.begin(), vertices.end(), other.vertices.begin());
}

bool Shape::operator<(const Shape& other) const {
    return computeShapeArea(*this) < computeShapeArea(other);
}

std::istream& operator>>(std::istream& in, Vertex& v) {
    char ch;
    if (in >> ch && ch == '(' && in >> v.x >> ch && ch == ';' && in >> v.y >> ch && ch == ')') {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Shape& shape) {
    shape.vertices.clear();
    int vertexCount;

    if (!(in >> vertexCount) || vertexCount < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    for (int i = 0; i < vertexCount; ++i) {
        Vertex v;
        if (!(in >> v)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        shape.vertices.push_back(v);
    }
    return in;
}

double calculatePolygonArea(const std::vector<Vertex>& vertices) {
    double area = 0.0;
    size_t n = vertices.size();

    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        area += vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
    }

    return std::abs(area) / 2.0;
}

double computeShapeArea(const Shape& shape) {
    return calculatePolygonArea(shape.vertices);
}

double AreaAccumulator::operator()(double sum, const Shape& shape) const {
    return sum + computeShapeArea(shape);
}

double FilteredAreaAccumulator::operator()(double sum, const Shape& shape) const {
    bool condition = (parity == VertexParity::EVEN) ?
        (shape.vertices.size() % 2 == 0) :
        (shape.vertices.size() % 2 == 1);
    return condition ? sum + computeShapeArea(shape) : sum;
}

double VertexCountAreaAccumulator::operator()(double sum, const Shape& shape) const {
    return shape.vertices.size() == count ? sum + computeShapeArea(shape) : sum;
}
