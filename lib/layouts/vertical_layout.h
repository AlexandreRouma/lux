#pragma once
#include "../widget.h"
#include <string>

namespace lux {
    class VerticalLayout : public Widget {
    public:
        VerticalLayout(Widget* parent);

        void setContainerSize(const Size& containerSize);

        void addElement(const std::shared_ptr<Widget>& element);

    private:
        void draw();
        void updateElementSizes();

        std::vector<std::shared_ptr<Widget>> elements;
    };
}