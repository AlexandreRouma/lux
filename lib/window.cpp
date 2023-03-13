#include "window.h"
#include "backend.h"
#include "flog.h"

namespace lux {
    Window::Window(const Size& size, const std::string& title, const Point& position) {
        this->size = size;
        this->title = title;
        this->position = position;
        drawList = std::make_shared<DrawList>();
        updateButtonPositions();
        backend::registerWindow(this);
    }

    Window::~Window() {
        close();
    }

    void Window::close() {
        // Only attempt to close if window is open
        if (!open) { return; }
        open = false;

        // Unregister from backend
        backend::unregisterWindow(this);
    }

    const Size& Window::getSize() {
        return size;
    }

    void Window::setSize(const Size& size, bool notifyBackend) {
        redraw = true;
        this->size = size;
        updateButtonPositions();
        if (notifyBackend) { backend::resizeWindow(this, size); }
    }

    const std::string Window::getTitle() { return title; }
    const Point& Window::getPosition() { return position; }
    const Color& Window::getBackgroundColor() { return background; }

    bool Window::redrawRequired() {
        return redraw;
    }

    const std::shared_ptr<DrawList>& Window::getDrawList() {
        if (redraw) {
            redraw = false;
            draw();
        }
        return drawList;
    }

    void Window::gainFocus() {
        focused = true;
        redraw = true;
    }

    void Window::loseFocus() {
        focused = false;
        redraw = true;
    }

    void Window::mouseEnter() {
        //flog::info("[{}] Mouse enter", title);
    }

    void Window::mouseLeave() {
        if (minimizeHovered || maximizeHovered || closeHovered) { redraw = true; }
        if (minimizeClicked || maximizeClicked || closeClicked) { redraw = true; }
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
        if (minimizeClicked != _minimizeClicked) { redraw = true; }
        if (maximizeClicked != _maximizeClicked) { redraw = true; }
        if (closeClicked != _closeClicked) { redraw = true; }
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
        if (minimizeClicked) {

        }
        else if (maximizeClicked) {

        }
        else if (closeClicked) {
            close();
        }

        mouseDownInTitle = false;

        if (minimizeClicked || maximizeClicked || closeClicked) { redraw = true; }
        minimizeClicked = false;
        maximizeClicked = false;
        closeClicked = false;
        
        //flog::info("[{}] Mouse up", title);
    }

    void Window::mouseMove(const Point& mpos) {
        if (mouseDownInTitle) {
            auto delta = mpos - mouseTitlePos;
            position = position + delta;
            backend::moveWindow(this, position);
        }

        bool _minimizeHovered = (mpos.x >= minimizeP1.x && mpos.y >= minimizeP1.y && mpos.x < minimizeP2.x && mpos.y < minimizeP2.y);
        bool _maximizeHovered = (mpos.x >= maximizeP1.x && mpos.y >= maximizeP1.y && mpos.x < maximizeP2.x && mpos.y < maximizeP2.y);
        bool _closeHovered = (mpos.x >= closeP1.x && mpos.y >= closeP1.y && mpos.x < closeP2.x && mpos.y < closeP2.y);
        if (minimizeHovered != _minimizeHovered) { redraw = true; }
        if (maximizeHovered != _maximizeHovered) { redraw = true; }
        if (closeHovered != _closeHovered) { redraw = true; }
        minimizeHovered = _minimizeHovered;
        maximizeHovered = _maximizeHovered;
        closeHovered = _closeHovered;

        //flog::info("[{}] Mouse move ({}, {})", title, mpos.x, mpos.y);
    }

    void Window::draw() {
        drawList->clear();

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
    }

    void Window::updateButtonPositions() {
        minimizeP1 = Point(size.x - 138, 0);
        minimizeP2 = minimizeP1 + Point(46, 30);
        maximizeP1 = Point(size.x - 93, 0);;
        maximizeP2 = maximizeP1 + Point(46, 30);
        closeP1 = Point(size.x - 47, 0);
        closeP2 = closeP1 + Point(46, 30);;
    }
}