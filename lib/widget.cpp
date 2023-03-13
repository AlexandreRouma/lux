#include "widget.h"

namespace lux {
    Widget::Widget(Widget* parent) {
        drawList = std::make_shared<DrawList>();
        this->parent = parent;
    }

    Widget::~Widget() {}

    void Widget::setContainerSize(const Size& containerSize) {
        this->containerSize = containerSize;
    }

    const Size& Widget::getSize() {
        return size;
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
}