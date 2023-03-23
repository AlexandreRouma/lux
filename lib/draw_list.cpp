#include "draw_list.h"
#include "vendor/stb/stb_truetype.h"
#include "lux.h"

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

    void DrawList::drawText(const Point& p, const Color& color, const std::string& text) {
        newElement();
        elem->texId = fontTexId;
        Vec2f pos = Vec2f((float)p.x - 0.5f, (float)p.y - 0.5f);
        for (char c : text) {
            // If new line, go to next line
            if (c == '\n') {
                pos.x = (float)p.x - 0.5f;
                pos.y += font->getHeight();
                continue;
            }

            // Get info
            stbtt_bakedchar bc = font->bcs[c];

            // Draw quad
            Vec2f cpos = pos + Vec2f(bc.xoff, bc.yoff);
            Vec2f end(cpos.x + (float)(bc.x1 - bc.x0), cpos.y + (float)(bc.y1 - bc.y0));
            int tl = elem->addVertex(cpos, color, Vec2f(bc.x0, bc.y0) * (1.0f/511.0f));
            int tr = elem->addVertex(Vec2f(end.x, cpos.y), color, Vec2f(bc.x1, bc.y0) * (1.0f/511.0f));
            int bl = elem->addVertex(Vec2f(cpos.x, end.y), color, Vec2f(bc.x0, bc.y1) * (1.0f/511.0f));
            int br = elem->addVertex(end, color, Vec2f(bc.x1, bc.y1) * (1.0f/511.0f));
            elem->addTri(tl, tr, bl);
            elem->addTri(tr, bl, br);
                
            // Increment X pos
            pos.x += bc.xadvance;
        }
        newElement();
    }

    void DrawList::drawList(const Point& p, const std::shared_ptr<DrawList>& list) {
        newElement();
        elem->drawListPos = p;
        elem->drawList = list;
        newElement();
    }

    void DrawList::newElement() {
        elements.push_back(DrawElement());
        elem = &elements.back();
    }
}