#include "vertical_layout.h"
#include "flog.h"

namespace lux {
    VerticalLayout::VerticalLayout(Widget* parent) : Widget(parent) {}

    void VerticalLayout::setContainerSize(const Size& containerSize) {
        size = containerSize;
        updateElementSizes();
        markForRedraw();
    }

    void VerticalLayout::addElement(const std::shared_ptr<Widget>& element) {
        elements.push_back(element);
        updateElementSizes();
        markForRedraw();
    }

    void VerticalLayout::draw() {
        drawList->clear();
        drawList->setDrawArea(size);

        Point pos(0, 0);
        for (auto& element : elements) {
            drawList->drawList(pos, element->getDrawList());
            pos = pos + Point(0, element->getSize().y + 5);
        }
    }

    void VerticalLayout::updateElementSizes() {
        for (auto& element : elements) {
            element->setContainerSize(size);
        }
    }
};