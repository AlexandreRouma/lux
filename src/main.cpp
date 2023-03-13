#include <stdio.h>
#include "lux.h"
#include "window.h"
#include "flog.h"
#include "widgets/button.h"

class MainWindow : public lux::Window {
public:
    MainWindow() : lux::Window(lux::Point(1280, 720), "Hello Lux!", lux::Point(500, 300)) {
        btn = std::make_shared<lux::Button>(this, "Click Me!");
        setRootWidget(btn);
    }

private:
    std::shared_ptr<lux::Button> btn;
};

int main() {
    MainWindow mainWindow;
    lux::run();
    return 0;
}