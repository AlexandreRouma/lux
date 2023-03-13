#pragma once
#include "opengl_drawer.h"
#include "../backend.h"
#include <GLFW/glfw3.h>


namespace lux::backend {
    struct BackendWindow {
        Window* luxWin;
        GLFWwindow* glfwWin;
        std::shared_ptr<OpenGLDrawer> drawer;
        Point mpos = Point(-1, -1);
        bool mouseIn = false;
        bool mouseDown = false;
    };
};