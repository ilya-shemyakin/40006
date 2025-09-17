#include "CommandHandlers.h"
#include <numeric>
#include <algorithm>
#include <functional>
#include <iostream>

double calculateEvenOddArea(const std::vector<Shape>& shapes, VertexParity parity) {
    FilteredAreaAccumulator accumulator{ parity };
    return std::accumulate(shapes.begin(), shapes.end(), 0.0, accumulator);
}

double calculateAverageArea(const std::vector<Shape>& shapes) {
    if (shapes.empty()) return 0.0;

    AreaAccumulator accumulator;
    double total = std::accumulate(shapes.begin(), shapes.end(), 0.0, accumulator);
    return total / shapes.size();
}

double calculateAreaByVertexCount(const std::vector<Shape>& shapes, int count) {
    VertexCountAreaAccumulator accumulator{ count };
    return std::accumulate(shapes.begin(), shapes.end(), 0.0, accumulator);
}

double findExtremeArea(const std::vector<Shape>& shapes, bool findMax) {
    auto comparator = [](const Shape& a, const Shape& b) {
        return computeShapeArea(a) < computeShapeArea(b);
        };

    auto it = findMax ?
        std::max_element(shapes.begin(), shapes.end(), comparator) :
        std::min_element(shapes.begin(), shapes.end(), comparator);

    return computeShapeArea(*it);
}

int findExtremeVertexCount(const std::vector<Shape>& shapes, bool findMax) {
    auto comparator = [](const Shape& a, const Shape& b) {
        return a.vertices.size() < b.vertices.size();
        };

    auto it = findMax ?
        std::max_element(shapes.begin(), shapes.end(), comparator) :
        std::min_element(shapes.begin(), shapes.end(), comparator);

    return it->vertices.size();
}

bool ParityPredicate::operator()(const Shape& shape) const {
    bool condition = (parity == VertexParity::EVEN) ?
        (shape.vertices.size() % 2 == 0) :
        (shape.vertices.size() % 2 == 1);
    return condition;
}

bool VertexCountPredicate::operator()(const Shape& shape) const {
    return shape.vertices.size() == count;
}

int countShapesByParity(const std::vector<Shape>& shapes, VertexParity parity) {
    ParityPredicate predicate{ parity };
    return std::count_if(shapes.begin(), shapes.end(), predicate);
}

int countShapesByVertexCount(const std::vector<Shape>& shapes, int count) {
    VertexCountPredicate predicate{ count };
    return std::count_if(shapes.begin(), shapes.end(), predicate);
}

void handleInvalidCommand() {
    std::cout << "<INVALID COMMAND>\n";
}
