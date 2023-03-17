#include "button.h"
#include "flog.h"
#include "../lux.h"
#include <math.h>

namespace lux {
    Button::Button(Widget* parent, const std::string& label, const Size& size) : Widget(parent) {
        this->label = label;
        setSize(size);
    }

    Button::~Button() {}

    Size Button::getFitContentSize() {
        return Size(font->calcTextSize(label) + 14, font->getHeight() + 14);
    }

    void Button::draw() {
        drawList->clear();
        drawList->setDrawArea(size);

        auto barColor = lux::Color(60.0/255.0, 60.0/255.0, 60.0/255.0);
        auto borderColor = lux::Color(30.0/255.0, 30.0/255.0, 30.0/255.0);

        int textBase = ceil((float)(size.y + font->getHeight()) / 2.0f);

        drawList->fillRect(lux::Point(0, 0), size, barColor);
        drawList->drawRect(lux::Point(0, 0), size, borderColor);
        drawList->drawText(lux::Point(7, textBase), Color(1.0, 1.0, 1.0), label);
    }
}