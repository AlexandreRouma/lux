#include "glfw.h"
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "flog.h"

namespace lux::backend {
    bool isInit = false;
    GLFWmonitor* primaryMonitor;
    // TODO: Make this a map from lux::Window* to the struct... literally the best way
    std::vector<BackendWindow> windows;
    std::vector<Window*> toClose;

    void init() {
        // Make sure this only runs once
        if (isInit) { return; }
        isInit = true;

        // Init GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialized GLFW");
        }

        // Save primary monitor
        primaryMonitor = glfwGetPrimaryMonitor();
    }

    void registerWindow(Window* window) {
        // Init in case it's not already
        init();

        // Enable 8x antialiasing
        glfwWindowHint(GLFW_SAMPLES, 8);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        // Select OpenGL version
#ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

        // Create window
        auto size = window->getSize();
        auto pos = window->getPosition();
        auto title = window->getTitle();
        GLFWwindow* gwindow = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);

        // Set position if required
        if (pos.x >= 0 && pos.y >= 0) {
            glfwSetWindowPos(gwindow, pos.x, pos.y);
        }

        // Bind OpenGL context
        glfwMakeContextCurrent(gwindow);

        // Init GLAD
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        // Add to list
        BackendWindow win;
        win.luxWin = window;
        win.glfwWin = gwindow;
        win.drawer = std::make_shared<OpenGLDrawer>(size);
        windows.push_back(win);
    }

    void unregisterWindow(Window* window) {
        // TODO: Check that it exists first
        toClose.push_back(window);
    }

    void destroyWindow(const BackendWindow& win) {
        glfwDestroyWindow(win.glfwWin);
    }

    void resizeWindow(Window* window, const Size& size) {
        // Get entry
        auto it = std::find_if(windows.begin(), windows.end(), [window](BackendWindow& bwin) { return bwin.luxWin == window; });
        if (it == windows.end()) { return; }
        auto win = *it;

        // Update size
        glfwSetWindowSize(win.glfwWin, size.x, size.y);

        // Update drawer
        win.drawer->setSize(size);
    }

    void moveWindow(Window* window, const Point& position) {
        // Get entry
        auto it = std::find_if(windows.begin(), windows.end(), [window](BackendWindow& bwin) { return bwin.luxWin == window; });
        if (it == windows.end()) { return; }
        auto win = *it;

        // Update position
        glfwSetWindowPos(win.glfwWin, position.x, position.y);
    }

    void minimizeWindow(Window* window) {
        // Get entry
        auto it = std::find_if(windows.begin(), windows.end(), [window](BackendWindow& bwin) { return bwin.luxWin == window; });
        if (it == windows.end()) { return; }
        auto win = *it;

        // Minimize
        glfwIconifyWindow(win.glfwWin);
    }

    void maximizeWindow(Window* window) {
        // Get entry
        auto it = std::find_if(windows.begin(), windows.end(), [window](BackendWindow& bwin) { return bwin.luxWin == window; });
        if (it == windows.end()) { return; }
        auto win = *it;

        // Minimize
        glfwMaximizeWindow(win.glfwWin);
    }

    void run() {
        while (!windows.empty()) {
            // Poll events
            glfwPollEvents();
            
            // Handle events for each window
            for (auto& win : windows) {
                // Check if the window should exit
                if (glfwWindowShouldClose(win.glfwWin)) {
                    win.luxWin->close();
                }

                // Check if window should be moved
                Point gpos;
                const Point& pos = win.luxWin->getPosition();
                glfwGetWindowPos(win.glfwWin, &gpos.x, &gpos.y);
                if (pos != gpos) {
                    win.luxWin->setPosition(gpos, false);
                }

                // Check if the window should be resized
                Size gsize;
                const Size& size = win.luxWin->getSize();
                glfwGetWindowSize(win.glfwWin, &gsize.x, &gsize.y);
                if (gsize.x > 0 && gsize.y > 0 && size != gsize) {
                    win.luxWin->setSize(gsize);
                    win.drawer->setSize(gsize);
                }

                // Get mouse position
                double mx, my;
                glfwGetCursorPos(win.glfwWin, &mx, &my);
                Point mpos(round(mx), round(my));

                // Check focus
                bool focused = glfwGetWindowAttrib(win.glfwWin, GLFW_FOCUSED);
                if (!win.focused && focused) {
                    win.focused = true;
                    win.luxWin->gainFocus();
                }
                else if (win.focused && !focused) {
                    win.focused = false;
                    win.luxWin->loseFocus();
                }

                // Detect mouse enter and mouse leave
                // TODO: Has to be only if the region of the window is visible, same thing for click
                bool mouseIn = (mpos.x >= 0 && mpos.y >= 0 && mpos.x < size.x && mpos.y < size.y);
                if (!win.mouseIn && mouseIn) {
                    win.mouseIn = true;
                    win.luxWin->mouseEnter();
                }
                else if (win.mouseIn && !mouseIn) {
                    win.mouseIn = false;
                    win.luxWin->mouseLeave();
                }

                // Check if mouse is down or not
                bool mouseDown = glfwGetMouseButton(win.glfwWin, GLFW_MOUSE_BUTTON_LEFT);
                if (!win.mouseDown && mouseDown) {
                    win.mouseDown = true;
                    win.luxWin->mouseDown(mpos);
                }
                else if (win.mouseDown && !mouseDown) {
                    win.mouseDown = false;
                    win.luxWin->mouseUp(mpos);
                }

                // Detect mouse mouvenet
                if ((mouseIn || mouseDown) && win.mpos != mpos) {
                    win.mpos = mpos;
                    win.luxWin->mouseMove(mpos);
                }
            }

            // Draw each window
            for (auto& win : windows) {
                // Check if a redraw is actually needed
                //if (!win.luxWin->redrawRequired()) { continue; }
                
                // Bind OpenGL context
                glfwMakeContextCurrent(win.glfwWin);

                // Draw
                win.drawer->draw(win.luxWin->getDrawList(), win.luxWin->getBackgroundColor());
                win.swapRequired = true;
            }

            // Configure
            glfwSwapInterval(true);

            // Swap buffers for each window
            for (auto& win : windows) {
                if (!win.swapRequired) { continue; }
                win.swapRequired = false;
                glfwSwapBuffers(win.glfwWin);
            }

            // Remove closed windows
            for (auto& win : toClose) {
                // Find window in the list
                auto it = std::find_if(windows.begin(), windows.end(), [win](BackendWindow& bwin) { return bwin.luxWin == win; });
                if (it == windows.end()) { continue; }

                // Bind context and destroy
                auto _win = *it;
                glfwMakeContextCurrent(_win.glfwWin);
                destroyWindow(_win);
                windows.erase(it);
            }
            if (!toClose.empty()) { toClose.clear(); }
        }
    }
}