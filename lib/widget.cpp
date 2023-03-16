#include "widget.h"

namespace lux {
    Widget::Widget(Widget* parent, const Size& requestedSize) {
        drawList = std::make_shared<DrawList>();
        this->parent = parent;
        setSize(requestedSize);
    }

    Widget::~Widget() {}

    Size getContainerSize() {
        
    }

    void Widget::setContainerSize(const Size& containerSize) {
        this->containerSize = containerSize;
        void computeSize();
    }

    const Size& Widget::getSize() {
        return size;
    }

    void Widget::setSize(const Size& requestedSize) {
        this->requestedSize = requestedSize;
        void computeSize();
    }

    void Widget::markForRedraw() {
        redraw = true;
        if (parent) { parent->markForRedraw(); }
    }

    bool Widget::redrawRequired() {
        return redraw;
    }

    const std::shared_ptr<DrawList>& Widget::getDrawList() {
        if (redraw) {
            redraw = false;
            draw();
        }
        return drawList;
    }

    void Widget::computeSize() {
        if (requestedSize.x == FIT_CONTAINER) {
            size.x = containerSize.x;
        }
        else if (requestedSize.x == FIT_CONTENT) {
            // TODO
        }
        else {
            size.x = requestedSize.x;
        }

        if (requestedSize.y == FIT_CONTAINER) {
            size.y = containerSize.y;
        }
        else if (requestedSize.y == FIT_CONTENT) {
            // TODO
        }
        else {
            size.y = requestedSize.y;
        }
    }
}