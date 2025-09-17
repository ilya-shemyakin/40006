#ifndef ADDITIONAL_COMMANDS_H
#define ADDITIONAL_COMMANDS_H

#include <vector>
#include "PolygonProcessor.h"

int countPermutationMatches(const std::vector<Shape>& shapes, const Shape& target);
int findMaxConsecutiveSequence(const std::vector<Shape>& shapes, const Shape& target);
bool isShapePermutation(const Shape& a, const Shape& b);

struct PermutationPredicate {
    Shape target;
    bool operator()(const Shape& shape) const;
};

struct ConsecutiveSequenceFinder {
    Shape target;
    int current;
    int max;

    ConsecutiveSequenceFinder(const Shape& t) : target(t), current(0), max(0) {}
    void operator()(const Shape& shape);
    int getResult() const { return max; }
};

#endif
