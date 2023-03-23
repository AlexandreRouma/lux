#include "button.h"
#include "flog.h"
#include "../lux.h"
#include <math.h>

namespace lux {
    Button::Button(Widget* parent, const std::string& label, const Size& size) : Widget(parent) {
        this->label = label;
        labelSize = Size(font->calcTextWidth(label), font->getHeight());
        setSize(size);
    }

    Button::~Button() {}

    Size Button::getFitContentSize() {
        return labelSize + Size(14, 14);
    }

    void Button::draw() {
        drawList->clear();
        drawList->setDrawArea(size);

        auto barColor = lux::Color(60.0/255.0, 60.0/255.0, 60.0/255.0);
        auto borderColor = lux::Color(30.0/255.0, 30.0/255.0, 30.0/255.0);

        drawList->fillRect(lux::Point(0, 0), size - Size(1, 1), barColor);
        drawList->drawRect(lux::Point(0, 0), size - Size(1, 1), borderColor);
        
        drawList->drawText(Size(7, 7 + labelSize.y), Color(1,1,1), label);
    }
}