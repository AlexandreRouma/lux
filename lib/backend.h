#pragma once
#include "window.h"
#include "vec2.h"

namespace lux::backend {
    void registerWindow(Window* window);
    void unregisterWindow(Window* window);
    void resizeWindow(Window* window, const Size& size);
    void moveWindow(Window* window, const Point& position);
    void minimizeWindow(Window* window);
    void maximizeWindow(Window* window);
    void run();
}