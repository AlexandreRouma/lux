#include "color.h"

namespace lux {
    Color::Color(float r, float g, float b) :
        Color(r, g, b, 1.0f)
    {}

    Color::Color(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
}