#pragma once
#include "point.h"
#include "color.h"
#include <vector>
#include <string>
#include <memory>

namespace lux {
    class DrawList;
    
    enum DrawOp {
        DRAW_OP_NONE,
        DRAW_OP_DRAW_LINE,
        DRAW_OP_DRAW_RECT,
        DRAW_OP_FILL_RECT,
        DRAW_OP_DRAW_LIST
    };
    
    class DrawStep {
    public:
        virtual ~DrawStep() {}
        DrawOp op;
        Point p;
        Point p1;
        Point p2;
        Color color;
        std::shared_ptr<DrawList> list;
    };

    class DrawList {
    public:
        DrawList();
        DrawList(const DrawList& other);

        bool empty();
        void clear();

        void drawLine(const Point& p1, const Point& p2, const Color& color);
        void drawRect(const Point& p1, const Point& p2, const Color& color);
        void fillRect(const Point& p1, const Point& p2, const Color& color);
        void drawList(const Point& p, const std::shared_ptr<DrawList>& list);

        const std::vector<DrawStep>& getSteps() const;

    private:
        std::vector<DrawStep> steps;
    };
}