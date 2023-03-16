#include "widget.h"
#include <stdexcept>

namespace lux {
    Widget::Widget(Widget* parent) {
        drawList = std::make_shared<DrawList>();
        this->parent = parent;
    }

    Widget::~Widget() {}

    const Size& Widget::getSize() {
        return size;
    }

    void Widget::setSize(const Size& requestedSize) {
        this->requestedSize = requestedSize;
        computeSize();
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
        Size newSize = size;

        // Get available size if possible
        if (parent) {
            availableSize = parent->getAvailableSize(this);
        }
        else if (requestedSize.x == FIT_CONTAINER || requestedSize.y == FIT_CONTAINER) {
            throw std::runtime_error("Cannot fit to container size widget has no parent");
        }

        // Get fit content size
        childrenComputeSize();
        Size fitContentSize = getFitContentSize();

        // X Size
        if (requestedSize.x == FIT_CONTAINER) {
            newSize.x = availableSize.x;         
        }
        else if (requestedSize.x == FIT_CONTENT) {
            newSize.x = fitContentSize.x;
        }
        else {
            newSize.x = requestedSize.x;
        }

        // Y Size
        if (requestedSize.y == FIT_CONTAINER) {
            newSize.y = availableSize.y;         
        }
        else if (requestedSize.y == FIT_CONTENT) {
            newSize.y = fitContentSize.y;
        }
        else {
            newSize.y = requestedSize.y;
        }

        // If size changed, notify parents and children
        if (size != newSize) {
            size = newSize;
            if (parent) { parent->computeSize(); }
            childrenComputeSize();
            markForRedraw();
        }
    }

    Size Widget::getAvailableSize(Widget* child) {
        throw std::runtime_error("Widget cannot be a parent");
    }

    void Widget::childrenComputeSize() {}
}