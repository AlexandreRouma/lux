#include "draw_list.h"

namespace lux {
    DrawList::DrawList() {}

    DrawList::DrawList(const DrawList& other) {
        steps = other.steps;
    }

    bool DrawList::empty() {
        return steps.empty();
    }

    void DrawList::clear() {
        steps.clear();
    }

    void DrawList::setDrawArea(const Size& drawArea) {
        this->drawArea = drawArea;
    }

    const Size& DrawList::getDrawArea() {
        return drawArea;
    }

    void DrawList::drawLine(const Point& p1, const Point& p2, const Color& color) {
        DrawStep step;
        step.op = DRAW_OP_DRAW_LINE;
        step.p1 = p1;
        step.p2 = p2;
        step.color = color;
        steps.push_back(step);
    }

    void DrawList::drawRect(const Point& p1, const Point& p2, const Color& color) {
        DrawStep step;
        step.op = DRAW_OP_DRAW_RECT;
        step.p1 = p1;
        step.p2 = p2;
        step.color = color;
        steps.push_back(step);
    }

    void DrawList::fillRect(const Point& p1, const Point& p2, const Color& color) {
        DrawStep step;
        step.op = DRAW_OP_FILL_RECT;
        step.p1 = p1;
        step.p2 = p2;
        step.color = color;
        steps.push_back(step);
    }

    void DrawList::drawText(const Point& p, const Color& color, const std::string& text) {
        DrawStep step;
        step.op = DRAW_OP_DRAW_TEXT;
        step.p = p;
        step.color = color;
        step.text = text;
        steps.push_back(step);
    }

    void DrawList::drawList(const Point& p, const std::shared_ptr<DrawList>& list) {
        DrawStep step;
        step.op = DRAW_OP_DRAW_LIST;
        step.p = p;
        step.list = list;
        steps.push_back(step);
    }

    const std::vector<DrawStep>& DrawList::getSteps() const {
        return steps;
    }
}