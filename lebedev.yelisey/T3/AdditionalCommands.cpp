#include "AdditionalCommands.h"
#include <algorithm>
#include <functional>
#include <iterator>

bool isShapePermutation(const Shape& a, const Shape& b) {
    if (a.vertices.size() != b.vertices.size()) return false;

    std::vector<Vertex> aSorted = a.vertices;
    std::vector<Vertex> bSorted = b.vertices;

    auto vertexComparator = [](const Vertex& v1, const Vertex& v2) {
        return v1.x < v2.x || (v1.x == v2.x && v1.y < v2.y);
        };

    std::sort(aSorted.begin(), aSorted.end(), vertexComparator);
    std::sort(bSorted.begin(), bSorted.end(), vertexComparator);

    return std::equal(aSorted.begin(), aSorted.end(), bSorted.begin());
}

bool PermutationPredicate::operator()(const Shape& shape) const {
    return isShapePermutation(shape, target);
}

void ConsecutiveSequenceFinder::operator()(const Shape& shape) {
    if (shape == target) {
        current++;
        max = std::max(max, current);
    }
    else {
        current = 0;
    }
}

int countPermutationMatches(const std::vector<Shape>& shapes, const Shape& target) {
    PermutationPredicate predicate{ target };
    return std::count_if(shapes.begin(), shapes.end(), predicate);
}

int findMaxConsecutiveSequence(const std::vector<Shape>& shapes, const Shape& target) {
    ConsecutiveSequenceFinder finder(target);
    finder = std::for_each(shapes.begin(), shapes.end(), finder);
    return finder.getResult();
}
