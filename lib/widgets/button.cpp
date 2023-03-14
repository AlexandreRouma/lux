#include "button.h"
#include "flog.h"

namespace lux {
    Button::Button(Widget* parent, const std::string& label, const Size& size) : Widget(parent) {
        this->size = size;
        if (!this->size.y) { this->size.y = 25; }
    }

    void Button::setContainerSize(const Size& containerSize) {
        Widget::setContainerSize(containerSize);
        size.x = containerSize.x;
        markForRedraw();
    }

    void Button::draw() {
        drawList->clear();
        drawList->setDrawArea(containerSize);

        auto barColor = lux::Color(60.0/255.0, 60.0/255.0, 60.0/255.0);
        auto borderColor = lux::Color(30.0/255.0, 30.0/255.0, 30.0/255.0);

        drawList->fillRect(lux::Point(0, 0), size, barColor);
        drawList->drawRect(lux::Point(0, 0), size, borderColor);
    }
}