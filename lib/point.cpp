#include "point.h"

namespace lux {
    Point::Point() {
        x = 0;
        y = 0;
    }

    Point::Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Point Point::operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point Point::operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    bool Point::operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool Point::operator!=(const Point& other) const {
        return x != other.x || y != other.y;
    }
}