#pragma once
#include "../widget.h"
#include <string>

namespace lux {
    class VerticalLayout : public Widget {
    public:
        VerticalLayout(Widget* parent, const Color& color, const Size& size);
        ~VerticalLayout();

        void setChild(const std::shared_ptr<Widget>& child);

    private:
        Size getAvailableSize(Widget* child);
        Size getFitContentSize();
        void childrenComputeSize();

        void draw();

        Color color;
        std::shared_ptr<Widget> child;
    };
}