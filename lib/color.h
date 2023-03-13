#pragma once

namespace lux {
    struct Color {
        Color() {}
        Color(float r, float g, float b);
        Color(float r, float g, float b, float a);
        
        float r, g, b, a;
    };
}