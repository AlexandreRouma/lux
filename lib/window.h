#pragma once
#include <string>
#include <memory>
#include "point.h"
#include "color.h"
#include "draw_list.h"

namespace lux {
    class Window {
    public:
        Window(const Size& size, const std::string& title, const Point& position = Point(-1, -1));
        ~Window();

        void close();

        const Size& getSize();
        void setSize(const Size& size, bool notifyBackend = true);

        const std::string getTitle();
        const Point& getPosition();
        const Color& getBackgroundColor();

        const std::shared_ptr<DrawList>& getDrawList();

        void mouseEnter();
        void mouseLeave();
        void mouseDown(const Point& mpos);
        void mouseUp(const Point& mpos);
        void mouseMove(const Point& mpos);

    private:
        void draw();
        void updateButtonPositions();

        bool open = true;
        bool redraw = true;

        Point minimizeP1;
        Point minimizeP2;
        bool minimizeHovered = false;
        bool minimizeClicked = false;

        Point maximizeP1;
        Point maximizeP2;
        bool maximizeHovered = false;
        bool maximizeClicked = false;

        Point closeP1;
        Point closeP2;
        bool closeHovered = false;
        bool closeClicked = false;

        bool mouseDownInTitle = false;
        Point mouseTitlePos;

        Size size;
        std::string title;
        Point position;
        Color background = Color(0.1, 0.1, 0.1);
        std::shared_ptr<DrawList> drawList;
    };
}