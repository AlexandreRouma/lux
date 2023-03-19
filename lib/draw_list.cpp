#include "draw_list.h"

namespace lux {
    int DrawElement::addVertex(const Vec2f& pos, const Color& color, const Vec2f& texCoord) {
        VertexAttrib vert;
        vert.posAttr[0] = pos.x;
        vert.posAttr[1] = pos.y;
        vert.colorAttr[0] = color.r;
        vert.colorAttr[1] = color.g;
        vert.colorAttr[2] = color.b;
        vert.colorAttr[3] = color.a;
        vert.texCoordAttr[0] = texCoord.x;
        vert.texCoordAttr[1] = texCoord.y;
        vertices.push_back(vert);
        return vertices.size() - 1;
    }

    void DrawElement::addTri(int a, int b, int c) {
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
    }

    bool DrawElement::empty() {
        return indices.empty();
    }
    
    DrawList::DrawList() {
        newElement();
    }

    bool DrawList::empty() {
        return elements.empty();
    }

    void DrawList::clear() {
        elements.clear();
        newElement();
    }

    void DrawList::setDrawArea(const Size& drawArea) {
        this->drawArea = drawArea;
    }

    const Size& DrawList::getDrawArea() {
        return drawArea;
    }

    const std::vector<DrawElement>& DrawList::getElements() {
        return elements;
    }

    void DrawList::drawLine(const Point& p1, const Point& p2, const Color& color, int width) {
        Vec2f a = Vec2f(p1.x, p1.y);
        Vec2f b = Vec2f(p2.x, p2.y);
        Vec2f forw = b - a;
        forw = forw * 0.5f / forw.norm();
        Vec2f down(forw.y, -forw.x);
        down = down * (float)width;

        int tl = elem->addVertex(a - forw - down, color);
        int tr = elem->addVertex(b + forw - down, color);
        int bl = elem->addVertex(a - forw + down, color);
        int br = elem->addVertex(b + forw + down, color);

        elem->addTri(tl, tr, bl);
        elem->addTri(tr, bl, br);
    }

    void DrawList::drawRect(const Point& p1, const Point& p2, const Color& color) {
        int otl = elem->addVertex(Vec2f(p1.x, p1.y) + Vec2f(-0.5f, -0.5f), color);
        int otr = elem->addVertex(Vec2f(p2.x, p1.y) + Vec2f(0.5f, -0.5f), color);
        int obl = elem->addVertex(Vec2f(p1.x, p2.y) + Vec2f(-0.5f, 0.5f), color);
        int obr = elem->addVertex(Vec2f(p2.x, p2.y) + Vec2f(0.5f, 0.5f), color);
        int itl = elem->addVertex(Vec2f(p1.x, p1.y) + Vec2f(0.5f, 0.5f), color);
        int itr = elem->addVertex(Vec2f(p2.x, p1.y) + Vec2f(-0.5f, 0.5f), color);
        int ibl = elem->addVertex(Vec2f(p1.x, p2.y) + Vec2f(0.5f, -0.5f), color);
        int ibr = elem->addVertex(Vec2f(p2.x, p2.y) + Vec2f(-0.5f, -0.5f), color);

        // Top
        elem->addTri(otl, otr, itl);
        elem->addTri(otr, itl, itr);

        // Left
        elem->addTri(otl, itl, obl);
        elem->addTri(itl, obl, ibl);

        // Bottom
        elem->addTri(ibl, ibr, obl);
        elem->addTri(ibr, obl, obr);

        // Right
        elem->addTri(itr, otr, ibr);
        elem->addTri(otr, ibr, obr);
    }

    void DrawList::fillRect(const Point& p1, const Point& p2, const Color& color) {
        int tl = elem->addVertex(Vec2f(p1.x, p1.y) + Vec2f(-0.5f, -0.5f), color);
        int tr = elem->addVertex(Vec2f(p2.x, p1.y) + Vec2f(0.5f, -0.5f), color);
        int bl = elem->addVertex(Vec2f(p1.x, p2.y) + Vec2f(-0.5f, 0.5f), color);
        int br = elem->addVertex(Vec2f(p2.x, p2.y) + Vec2f(0.5f, 0.5f), color);

        elem->addTri(tl, tr, bl);
        elem->addTri(tr, bl, br);
    }

    // void DrawList::drawText(const Point& p, const Color& color, const std::string& text) {
        
    // }

    // void DrawList::drawList(const Point& p, const std::shared_ptr<DrawList>& list) {
        
    // }

    void DrawList::newElement() {
        elements.push_back(DrawElement());
        elem = &elements.back();
    }
}