#pragma once
#include "vec2.h"
#include "color.h"
#include <vector>
#include <string>
#include <memory>

namespace lux {
#pragma pack(push, 1)
    struct VertexAttrib {
        float posAttr[2];
        float colorAttr[4];
        float texCoordAttr[2];
    };
#pragma pack(pop)

    class DrawList;

    struct DrawElement {
        int addVertex(const Vec2f& pos, const Color& color, const Vec2f& texCoord = Vec2f(0.0f, 0.0f));
        void addTri(int a, int b, int c);
        bool empty();

        Point drawListPos;
        std::shared_ptr<DrawList> drawList;
        std::vector<VertexAttrib> vertices;
        std::vector<int> indices;
    };

    class DrawList {
    public:
        DrawList();

        bool empty();
        void clear();
        void setDrawArea(const Size& drawArea);
        const Size& getDrawArea();

        void drawLine(const Point& p1, const Point& p2, const Color& color, int width = 1);
        void drawRect(const Point& p1, const Point& p2, const Color& color);
        void fillRect(const Point& p1, const Point& p2, const Color& color);
        //void drawText(const Point& p, const Color& color, const std::string& text);
        void drawList(const Point& p, const std::shared_ptr<DrawList>& list);

        const std::vector<DrawElement>& getElements();

    private:
        void newElement();

        Size drawArea = Size(0, 0);
        DrawElement* elem;
        std::vector<DrawElement> elements;
    };
}