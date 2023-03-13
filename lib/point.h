#pragma once

namespace lux {
    struct Point {
        Point();
        Point(int x, int y);

        Point operator+(const Point& other) const;
        Point operator-(const Point& other) const;
        bool operator==(const Point& other) const;
        bool operator!=(const Point& other) const;

        int x, y;
    };

    // TODO: Rename struct to Vec2 and use two typedefs

    typedef Point Size;
}