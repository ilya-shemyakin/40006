#include "Geometry.h"
#include <iostream>
#include <algorithm>
#include <cmath>

bool Point::operator==(const Point& other) const {
    return x_ == other.x_ && y_ == other.y_;
}

bool Polygon::operator<(const Polygon& other) const {
    return calculateArea(this->points_) < calculateArea(other.points_);
}

bool Polygon::operator==(const Polygon& other) const {
    return points_ == other.points_;
}

std::istream& operator>>(std::istream& in, Point& p) {
    char c;
    if (in >> c && c == '(' && in >> p.x_ >> c && c == ';' && in >> p.y_ >> c && c == ')') {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly) {
    poly.points_.clear();
    int count;
    if (!(in >> count) || count < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }
    for (int i = 0; i < count; ++i) {
        Point p;
        if (!(in >> p)) {
            in.setstate(std::ios::failbit);
            return in;
        }
        poly.points_.push_back(p);
    }
    return in;
}

double calculateArea(const std::vector<Point>& points) {
    double area = 0.0;
    size_t n = points.size();

    for (size_t i = 0; i < n; i++) {
        int j = (static_cast<int>(i + 1)) % n;
        area += (points[i].x_ * points[j].y_) - (points[i].y_ * points[j].x_);
    }
    return std::abs(area) / 2.0;
}

bool EvenOddFilter::operator()(const Polygon& figure) const {
    return static_cast<int>(figure.points_.size()) % 2 == mod_;
}

double AddArea(double acc, const Polygon& p) {
    return acc + calculateArea(p.points_);
}

double AddFilteredArea::apply(double acc, const Polygon& p) const {
    int size = static_cast<int>(p.points_.size());
    if ((parity_ == Parity::EVEN && size % 2 == 0) ||
        (parity_ == Parity::ODD && size % 2 == 1)) {
        return acc + calculateArea(p.points_);
    }
    return acc;
}

double AddSpecificVertexCountArea::apply(double acc, const Polygon& p) const {
    return (static_cast<int>(p.points_.size()) == n_) ? acc + calculateArea(p.points_) : acc;
}

bool isRectangle(const Polygon& poly) {
    if (poly.points_.size() != 4) {
        return false;
    }

    const auto& pts = poly.points_;
    
    std::vector<int> distances;
    for (size_t i = 0; i < 4; ++i) {
        size_t next = (i + 1) % 4;
        int dx = pts[i].x_ - pts[next].x_;
        int dy = pts[i].y_ - pts[next].y_;
        distances.push_back(dx * dx + dy * dy);
    }

    if (distances[0] != distances[2] || distances[1] != distances[3]) {
        return false;
    }

    for (size_t i = 0; i < 4; ++i) {
        size_t prev = (i + 3) % 4;
        size_t next = (i + 1) % 4;
        
        int dx1 = pts[i].x_ - pts[prev].x_;
        int dy1 = pts[i].y_ - pts[prev].y_;
        int dx2 = pts[next].x_ - pts[i].x_;
        int dy2 = pts[next].y_ - pts[i].y_;
        
        int dot = dx1 * dx2 + dy1 * dy2;
        if (dot != 0) {
            return false;
        }
    }

    return true;
}

int orientation(const Point& p, const Point& q, const Point& r) {
    int val = (q.y_ - p.y_) * (r.x_ - q.x_) - (q.x_ - p.x_) * (r.y_ - q.y_);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

bool onSegment(const Point& p, const Point& q, const Point& r) {
    if (q.x_ <= std::max(p.x_, r.x_) && q.x_ >= std::min(p.x_, r.x_) &&
        q.y_ <= std::max(p.y_, r.y_) && q.y_ >= std::min(p.y_, r.y_)) {
        return true;
    }
    return false;
}

bool doSegmentsIntersect(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    int o1 = orientation(p1, p2, p3);
    int o2 = orientation(p1, p2, p4);
    int o3 = orientation(p3, p4, p1);
    int o4 = orientation(p3, p4, p2);

    if (o1 != o2 && o3 != o4) {
        return true;
    }

    if (o1 == 0 && onSegment(p1, p3, p2)) return true;
    if (o2 == 0 && onSegment(p1, p4, p2)) return true;
    if (o3 == 0 && onSegment(p3, p1, p4)) return true;
    if (o4 == 0 && onSegment(p3, p2, p4)) return true;

    return false;
}

bool isPointInPolygon(const Point& point, const Polygon& poly) {
    int n = static_cast<int>(poly.points_.size());
    if (n < 3) return false;

    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((poly.points_[i].y_ > point.y_) != (poly.points_[j].y_ > point.y_)) &&
            (point.x_ < (poly.points_[j].x_ - poly.points_[i].x_) * (point.y_ - poly.points_[i].y_) /
            (poly.points_[j].y_ - poly.points_[i].y_) + poly.points_[i].x_)) {
            inside = !inside;
        }
    }
    return inside;
}

bool doPolygonsIntersect(const Polygon& poly1, const Polygon& poly2) {
    for (const auto& point : poly1.points_) {
        if (isPointInPolygon(point, poly2)) {
            return true;
        }
    }

    for (const auto& point : poly2.points_) {
        if (isPointInPolygon(point, poly1)) {
            return true;
        }
    }

    for (size_t i = 0; i < poly1.points_.size(); ++i) {
        size_t nextI = (i + 1) % poly1.points_.size();
        for (size_t j = 0; j < poly2.points_.size(); ++j) {
            size_t nextJ = (j + 1) % poly2.points_.size();
            if (doSegmentsIntersect(poly1.points_[i], poly1.points_[nextI],
                                  poly2.points_[j], poly2.points_[nextJ])) {
                return true;
            }
        }
    }

    return false;
}
