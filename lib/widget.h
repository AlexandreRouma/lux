#pragma once
#include "point.h"
#include "draw_list.h"

namespace lux {
    class Widget {
    public:
        Widget(Widget* parent);
        virtual ~Widget();

        virtual void setContainerSize(const Size& containerSize);
        const Size& getSize();

        // TODO: Optimize it to avoid multiple recursive call when already marked for redraw
        void markForRedraw();
        bool redrawRequired();
        const std::shared_ptr<DrawList>& getDrawList();

    protected:
        virtual void draw() = 0;

        Widget* parent;
        Size containerSize;
        Size size;
        bool redraw = true;
        std::shared_ptr<DrawList> drawList;
    };
}