#include "button.h"
#include "flog.h"

namespace lux {
    Button::Button(Widget* parent, const std::string& label, const Size& size) : Widget(parent) {
        setSize(size);
    }

    Button::~Button() {}

    Size Button::getFitContentSize() {
        return Size(100, 25);
    }

    void Button::draw() {
        drawList->clear();
        drawList->setDrawArea(size);

        auto barColor = lux::Color(60.0/255.0, 60.0/255.0, 60.0/255.0);
        auto borderColor = lux::Color(30.0/255.0, 30.0/255.0, 30.0/255.0);

        drawList->fillRect(lux::Point(0, 0), size, barColor);
        drawList->drawRect(lux::Point(0, 0), size, borderColor);
    }
}