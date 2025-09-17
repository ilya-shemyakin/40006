#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include <vector>
#include "PolygonProcessor.h"

double calculateEvenOddArea(const std::vector<Shape>& shapes, VertexParity parity);
double calculateAverageArea(const std::vector<Shape>& shapes);
double calculateAreaByVertexCount(const std::vector<Shape>& shapes, int count);
double findExtremeArea(const std::vector<Shape>& shapes, bool findMax);
int findExtremeVertexCount(const std::vector<Shape>& shapes, bool findMax);
int countShapesByParity(const std::vector<Shape>& shapes, VertexParity parity);
int countShapesByVertexCount(const std::vector<Shape>& shapes, int count);
void handleInvalidCommand();

struct ParityPredicate {
    VertexParity parity;
    bool operator()(const Shape& shape) const;
};

struct VertexCountPredicate {
    int count;
    bool operator()(const Shape& shape) const;
};

#endif
