#pragma once
#include "vec2.h"
#include "draw_list.h"
#include <limits.h>

namespace lux {
    enum SizeMode {
        FIT_CONTAINER   = -1,
        FIT_CONTENT     = 0,
        NO_LIMIT        = INT_MAX
    };

    class Widget {
    public:
        Widget(Widget* parent);
        virtual ~Widget();
        
        const Size& getSize();
        void setSize(const Size& requestedSize);

        // TODO: Optimize it to avoid multiple recursive call when already marked for redraw
        // TODO: Also make a different between drawing and mearly rerendering
        void markForRedraw();
        bool redrawRequired();
        const std::shared_ptr<DrawList>& getDrawList();

        // TODO: Hide from public view
        void computeSize();

    protected:

        virtual Size getAvailableSize(Widget* child);
        virtual Size getFitContentSize() = 0;
        // TODO: Make child list standard
        virtual void childrenComputeSize();

        virtual void draw() = 0;

        Widget* parent;
        Size availableSize;
        Size requestedSize;
        Size size;

        bool redraw = true;
        std::shared_ptr<DrawList> drawList;
    };
}