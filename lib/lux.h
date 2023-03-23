#pragma once
#include <memory>
#include "font.h"

namespace lux {
    // TODO: This shouldn't work like this
    extern GLuint fontTexId;
    extern std::shared_ptr<Font> font;

    void run();
};