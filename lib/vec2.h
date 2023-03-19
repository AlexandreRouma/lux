#pragma once

namespace lux {
    struct Vec2 {
        Vec2();
        Vec2(int x, int y);

        Vec2 operator+(const Vec2& other) const;
        Vec2 operator-(const Vec2& other) const;
        bool operator==(const Vec2& other) const;
        bool operator!=(const Vec2& other) const;

        int x, y;
    };

    struct Vec2f {
        Vec2f();
        Vec2f(float x, float y);

        float norm();

        Vec2f operator+(const Vec2f& other) const;
        Vec2f operator-(const Vec2f& other) const;
        Vec2f operator*(const float& other) const;
        Vec2f operator/(const float& other) const;
        bool operator==(const Vec2f& other) const;
        bool operator!=(const Vec2f& other) const;

        float x, y;
    };

    typedef Vec2 Point;
    typedef Vec2 Size;
}