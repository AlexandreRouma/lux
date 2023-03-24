#include "button.h"
#include "flog.h"
#include "../lux.h"
#include <math.h>

namespace lux {
    Button::Button(Widget* parent, const std::string& label, const Size& size) : Widget(parent) {
        this->label = label;
        labelSize = Size(font->calcTextWidth(label), font->ascent);
        setSize(size);
    }

    Button::~Button() {}

    Size Button::getFitContentSize() {
        return labelSize + Size(10, 10);
    }

    void Button::draw() {
        drawList->clear();
        drawList->setDrawArea(size);

        auto barColor = lux::Color(60.0/255.0, 60.0/255.0, 60.0/255.0);
        auto borderColor = lux::Color(30.0/255.0, 30.0/255.0, 30.0/255.0);

        drawList->fillRect(lux::Point(0, 0), size - Size(1, 1), barColor);
        
        drawList->drawText((Sizef)size / 2.0f, Color(1,1,1), label, HREF_CENTER, VREF_CENTER);
    }
}