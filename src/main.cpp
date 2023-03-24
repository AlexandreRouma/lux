#include <stdio.h>
#include "lux.h"
#include "window.h"
#include "flog.h"
#include <chrono>
#include <stdexcept>

#include "widgets/button.h"
#include "widgets/div.h"

class MainWindow : public lux::Window {
public:
    MainWindow() : lux::Window(lux::Point(800, 480), "Hello Lux!", lux::Point(500, 300)) {
        btn = std::make_shared<lux::Button>(this, "Hello World!", lux::Size(lux::FIT_CONTAINER, lux::FIT_CONTENT));
        setRootWidget(btn);
    }

private:
    std::shared_ptr<lux::Button> btn;
};

int main() {
    try {
        lux::font = std::make_shared<lux::Font>("../res/Roboto-Medium.ttf", 16); // ../res/Roboto-Medium.ttf
        MainWindow mainWindow;
        mainWindow.show();
        lux::run();
    }
    catch (std::runtime_error e) {
        flog::error("ERROR: {}", e.what());
    }
    return 0;
}