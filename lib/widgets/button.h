#pragma once
#include "../widget.h"
#include <string>

namespace lux {
    class Button : public Widget {
    public:
        Button(Widget* parent, const std::string& label, const Size& size = Size(200, 0));

        void setContainerSize(const Size& containerSize);

    private:
        void draw();
    };
}