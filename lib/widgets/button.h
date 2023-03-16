#pragma once
#include "../widget.h"
#include <string>

namespace lux {
    class Button : public Widget {
    public:
        Button(Widget* parent, const std::string& label, const Size& size = Size(FIT_CONTENT, FIT_CONTENT));
        ~Button();


    private:
        Size getFitContentSize();
        void draw();
    };
}