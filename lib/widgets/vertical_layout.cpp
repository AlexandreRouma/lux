#include "div.h"
#include "flog.h"

namespace lux {
    Div::Div(Widget* parent, const Color& color, const Size& size) : Widget(parent) {
        this->color = color;
        setSize(size);
    }

    Div::~Div() {}

    void Div::setChild(const std::shared_ptr<Widget>& child) {
        this->child = child;
        markForRedraw();
        computeSize();
    }

    Size Div::getAvailableSize(Widget* child) {
        auto avl = availableSize;
        if (requestedSize.x > 0) { avl.x = requestedSize.x; }
        if (requestedSize.y > 0) { avl.y = requestedSize.y; }
        return avl - Size(10, 10);
    }

    Size Div::getFitContentSize() {
        if (!child) { return Size(10, 10); }
        return child->getSize() + Size(10, 10);
    }

    void Div::childrenComputeSize() {
        if (child) { child->computeSize(); }
    }

    void Div::draw() {
        drawList->clear();
        drawList->setDrawArea(size);

        drawList->fillRect(lux::Point(0, 0), size - Size(1, 1), color);
        if (child) { drawList->drawList(lux::Point(5, 5), child->getDrawList()); }
    }
}