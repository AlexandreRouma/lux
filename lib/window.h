#pragma once
#include <string>
#include <memory>
#include "vec2.h"
#include "color.h"
#include "draw_list.h"
#include "widget.h"

namespace lux {
    class Window : public Widget{
    public:
        Window(const Size& size, const std::string& title, const Point& position = Point(-1, -1));
        virtual ~Window();

        virtual void show();
        virtual void close();
        virtual void minimize();
        virtual void maximize();

        const std::string& getTitle();
        void setTitle(const std::string& title);

        const Point& getPosition();
        void setPosition(const Point& position, bool updateBackend = true);

        const Color& getBackgroundColor();
        void setBackgroundColor(const Color& backgroundColor);
        
        void setRootWidget(const std::shared_ptr<Widget>& rootWidget);

        // Events
        virtual void gainFocus();
        virtual void loseFocus();
        virtual void mouseEnter();
        virtual void mouseLeave();
        virtual void mouseDown(const Point& mpos);
        virtual void mouseUp(const Point& mpos);
        virtual void mouseMove(const Point& mpos);

    private:
        Size getAvailableSize(Widget* child);
        Size getFitContentSize();
        void childrenComputeSize();

        void draw();
        void updateButtonPositions();

        bool focused = false;
        bool open = false;

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

        std::string title;
        Point position;
        Color background = Color(0.1, 0.1, 0.1);

        std::shared_ptr<Widget> rootWidget;
    };
}