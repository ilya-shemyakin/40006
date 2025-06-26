#ifndef FUNCTORS_H_
#define FUNCTORS_H_
#include "Func.h"


struct AreaComp {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return calculate_area(a.points) < calculate_area(b.points);
    }
};

struct VertsComp {
    bool operator()(const Polygon& a, const Polygon& b) const {
        return a.points.size() < b.points.size();
    }
};

struct IsEven {
    bool operator()(const Polygon& figure) const {
        return figure.points.size() % 2 == 0;
    }
};

struct IsOdd {
    bool operator()(const Polygon& figure) const {
        return figure.points.size() % 2 == 1;
    }
};

struct VertexCount {
    size_t count;
    VertexCount(size_t c) : count(c) {}
    bool operator()(const Polygon& figure) const {
        return figure.points.size() == count;
    }
};

#endif
