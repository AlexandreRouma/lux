#pragma once
#include "point.h"
#include "draw_list.h"

namespace lux {
    enum SizeMode {
        FIT_CONTAINER   = -1,
        FIT_CONTENT     = 0
    };

    class Widget {
    public:
        Widget(Widget* parent, const Size& size);
        virtual ~Widget();

        Size getContainerSize();
        void setContainerSize(const Size& containerSize);
        
        const Size& getSize();
        void setSize(const Size& requestedSize);

        // TODO: Optimize it to avoid multiple recursive call when already marked for redraw
        void markForRedraw();
        bool redrawRequired();
        const std::shared_ptr<DrawList>& getDrawList();


    protected:
        void computeSize();

        virtual void draw() = 0;

        Widget* parent;

        Size containerSize = Size(100, 100);
        Size requestedSize;
        Size size;

        bool redraw = true;
        std::shared_ptr<DrawList> drawList;
    };
}