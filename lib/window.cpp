#include "window.h"
#include "backend.h"
#include "flog.h"

namespace lux {
    Window::Window(const Size& size, const std::string& title, const Point& position) : Widget(NULL) {
        this->title = title;
        this->position = position;
        setSize(size);
        updateButtonPositions();
    }

    Window::~Window() {
        close();
    }

    void Window::show() {
        // Only attempt to show if window is closed
        if (open) { return; }
        open = true;

        backend::registerWindow(this);
    }

    void Window::close() {
        // Only attempt to close if window is open
        if (!open) { return; }
        open = false;

        // Unregister from backend
        backend::unregisterWindow(this);
    }

    void Window::minimize() {
        backend::minimizeWindow(this);
    }

    void Window::maximize() {
        //backend::maximizeWindow(this);
    }

    const std::string& Window::getTitle() {
        return title;
    }

    void Window::setTitle(const std::string& title) {
        this->title = title;
        markForRedraw();
    }

    const Point& Window::getPosition() {
        return position;
    }

    void Window::setPosition(const Point& position, bool updateBackend) {
        this->position = position;
        if (updateBackend) { backend::moveWindow(this, position); }
    }

    const Color& Window::getBackgroundColor() {
        return background;
    }

    void Window::setRootWidget(const std::shared_ptr<Widget>& rootWidget) {
        this->rootWidget = rootWidget;
        rootWidget->computeSize();
    }

    void Window::gainFocus() {
        focused = true;
        markForRedraw();
    }

    void Window::loseFocus() {
        focused = false;
        markForRedraw();
    }

    void Window::mouseEnter() {
        //flog::info("[{}] Mouse enter", title);
    }

    void Window::mouseLeave() {
        if (minimizeHovered || maximizeHovered || closeHovered) { markForRedraw(); }
        if (minimizeClicked || maximizeClicked || closeClicked) { markForRedraw(); }
        minimizeHovered = false;
        minimizeClicked = false;
        maximizeHovered = false;
        maximizeClicked = false;
        closeHovered = false;
        closeClicked = false;
        //flog::info("[{}] Mouse leave", title);
    }

    void Window::mouseDown(const Point& mpos) {
        bool _minimizeClicked = minimizeHovered;
        bool _maximizeClicked = maximizeHovered;
        bool _closeClicked = closeHovered;
        if (minimizeClicked != _minimizeClicked) { markForRedraw(); }
        if (maximizeClicked != _maximizeClicked) { markForRedraw(); }
        if (closeClicked != _closeClicked) { markForRedraw(); }
        minimizeClicked = _minimizeClicked;
        maximizeClicked = _maximizeClicked;
        closeClicked = _closeClicked;

        if (mpos.y <= 30 && !minimizeHovered && !maximizeHovered && !closeHovered) {
            mouseDownInTitle = true;
            mouseTitlePos = mpos;
        }

        //flog::info("[{}] Mouse down", title);
    }

    void Window::mouseUp(const Point& mpos) {
        if (minimizeClicked) { minimize(); }
        else if (maximizeClicked) {  maximize(); }
        else if (closeClicked) { close(); }

        mouseDownInTitle = false;

        if (minimizeClicked || maximizeClicked || closeClicked) { markForRedraw(); }
        minimizeClicked = false;
        maximizeClicked = false;
        closeClicked = false;
        
        //flog::info("[{}] Mouse up", title);
    }

    void Window::mouseMove(const Point& mpos) {
        if (mouseDownInTitle) {
            auto delta = mpos - mouseTitlePos;
            setPosition(position + delta);
        }

        bool _minimizeHovered = (mpos.x >= minimizeP1.x && mpos.y >= minimizeP1.y && mpos.x < minimizeP2.x && mpos.y < minimizeP2.y);
        bool _maximizeHovered = (mpos.x >= maximizeP1.x && mpos.y >= maximizeP1.y && mpos.x < maximizeP2.x && mpos.y < maximizeP2.y);
        bool _closeHovered = (mpos.x >= closeP1.x && mpos.y >= closeP1.y && mpos.x < closeP2.x && mpos.y < closeP2.y);
        if (minimizeHovered != _minimizeHovered) { markForRedraw(); }
        if (maximizeHovered != _maximizeHovered) { markForRedraw(); }
        if (closeHovered != _closeHovered) { markForRedraw(); }
        minimizeHovered = _minimizeHovered;
        maximizeHovered = _maximizeHovered;
        closeHovered = _closeHovered;

        //flog::info("[{}] Mouse move ({}, {})", title, mpos.x, mpos.y);
    }

    Size Window::getAvailableSize(Widget* child) {
        return size - Size(10, 10 + 30 + 1/*TODO: Find why +1?*/);
    }

    Size Window::getFitContentSize() {
        return size;
    }

    void Window::childrenComputeSize() {
        if (rootWidget) { rootWidget->computeSize(); }
    }

    void Window::draw() {
        updateButtonPositions();
        drawList->clear();
        drawList->setDrawArea(size);

        Color textColor;
        Color barColor;
        if (focused) {
            textColor = Color(1.0, 1.0, 1.0);
            barColor = lux::Color(60.0/255.0, 60.0/255.0, 60.0/255.0);
        }
        else {
            textColor = Color(0.8, 0.8, 0.8);
            barColor = lux::Color(50.0/255.0, 50.0/255.0, 51.0/255.0);
        }

        drawList->fillRect(lux::Point(0, 0), lux::Point(size.x - 1, 30), barColor);

        int minBtnX = size.x - 120;
        int maxBtnX = size.x - 74;
        int clsBtnX = size.x - 28;

        if (minimizeClicked) {
            drawList->fillRect(minimizeP1, minimizeP2, lux::Color(128.0/255.0, 128.0/255.0, 128.0/255.0));
        }
        else if (minimizeHovered) {
            drawList->fillRect(minimizeP1, minimizeP2, lux::Color(86.0/255.0, 86.0/255.0, 86.0/255.0));
        }
        else if (maximizeClicked) {
            drawList->fillRect(maximizeP1, maximizeP2, lux::Color(128.0/255.0, 128.0/255.0, 128.0/255.0));
        }
        else if (maximizeHovered) {
            drawList->fillRect(maximizeP1, maximizeP2, lux::Color(86.0/255.0, 86.0/255.0, 86.0/255.0));
        }
        else if (closeClicked) {
            drawList->fillRect(closeP1, closeP2, lux::Color(0.8, 0.0, 0.0));
        }
        else if (closeHovered) {
            drawList->fillRect(closeP1, closeP2, lux::Color(1.0, 0.0, 0.0));
        }

        drawList->drawLine(lux::Point(minBtnX, 14), lux::Point(minBtnX+9, 14), textColor);
        drawList->drawRect(lux::Point(maxBtnX, 9), lux::Point(maxBtnX+9, 18), textColor);
        drawList->drawLine(lux::Point(clsBtnX, 9), lux::Point(clsBtnX+9, 18), textColor);
        drawList->drawLine(lux::Point(clsBtnX, 18), lux::Point(clsBtnX+9, 9), textColor);

        // Border
        // drawList->drawRect(lux::Point(0, 0), size - lux::Point(10, 10), barColor);

        // drawList->drawText(lux::Point(9, 20), lux::Color(1.0, 1.0, 1.0), title);

        // srand(69);
        // for (int i = 0; i < 1000; i++) {
        //     if (i && i%5 == 0) drawList->newElement();
        //     auto p1 = Point(rand() % size.x, rand() % size.y);
        //     auto p2 = Point(rand() % size.x, rand() % size.y);
        //     drawList->drawRect(p1, p2, Color(1.0, 1.0, 1.0));
        // }


        // // Draw widget
        // if (rootWidget) { drawList->drawList(Point(5, 31 + 5), rootWidget->getDrawList()); }
    }

    void Window::updateButtonPositions() {
        // TODO: They overlap...
        minimizeP1 = Point(size.x - 138, 0);
        minimizeP2 = minimizeP1 + Point(46, 30);
        maximizeP1 = Point(size.x - 93, 0);;
        maximizeP2 = maximizeP1 + Point(46, 30);
        closeP1 = Point(size.x - 47, 0);
        closeP2 = closeP1 + Point(46, 30);;
    }
}