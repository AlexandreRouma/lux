#include <stdio.h>
#include "lux.h"
#include "window.h"
#include "flog.h"
#include "widgets/button.h"
#include "layouts/vertical_layout.h"

class MainWindow : public lux::Window {
public:
    MainWindow() : lux::Window(lux::Point(1280, 720), "Hello Lux!", lux::Point(500, 300)) {
        layout = std::make_shared<lux::VerticalLayout>(this);
        setRootWidget(layout);
        
        btn1 = std::make_shared<lux::Button>(layout.get(), "Button 1");
        layout->addElement(btn1);
        btn2 = std::make_shared<lux::Button>(layout.get(), "Button 2");
        layout->addElement(btn2);
        btn3 = std::make_shared<lux::Button>(layout.get(), "Button 3");
        layout->addElement(btn3);

        secWin = std::make_shared<lux::Window>(lux::Size(320, 240), "Second Window");
    }

    void gainFocus() {
        lux::Window::gainFocus();
        //secWin->show();
    }

private:
    std::shared_ptr<lux::VerticalLayout> layout;
    std::shared_ptr<lux::Button> btn1;
    std::shared_ptr<lux::Button> btn2;
    std::shared_ptr<lux::Button> btn3;

    std::shared_ptr<lux::Window> secWin;
};

int main() {
    MainWindow mainWindow;
    MainWindow mainWindow2;
    mainWindow.show();
    mainWindow2.show();
    lux::run();
    return 0;
}