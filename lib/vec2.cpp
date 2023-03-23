#include "vec2.h"
#include <math.h>

namespace lux {
    Vec2::Vec2() {
        x = 0;
        y = 0;
    }

    Vec2::Vec2(Vec2f vec2f) {
        x = vec2f.x;
        y = vec2f.y;
    }

    Vec2::Vec2(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Vec2 Vec2::operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 Vec2::operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    bool Vec2::operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }

    bool Vec2::operator!=(const Vec2& other) const {
        return x != other.x || y != other.y;
    }

    Vec2f::Vec2f() {
        x = 0.0f;
        y = 0.0f;
    }

    Vec2f::Vec2f(Vec2 vec2) {
        x = vec2.x;
        y = vec2.y;
    }

    Vec2f::Vec2f(float x, float y) {
        this->x = x;
        this->y = y;
    }

    float Vec2f::norm() {
        return sqrtf(x*x + y*y);
    }

    Vec2f Vec2f::operator+(const Vec2f& other) const {
        return Vec2f(x + other.x, y + other.y);
    }

    Vec2f Vec2f::operator-(const Vec2f& other) const {
        return Vec2f(x - other.x, y - other.y);
    }

    Vec2f Vec2f::operator*(const float& other) const {
        return Vec2f(x * other, y * other);
    }

    Vec2f Vec2f::operator/(const float& other) const {
        return Vec2f(x / other, y / other);
    }

    bool Vec2f::operator==(const Vec2f& other) const {
        return x == other.x && y == other.y;
    }

    bool Vec2f::operator!=(const Vec2f& other) const {
        return x != other.x || y != other.y;
    }
}