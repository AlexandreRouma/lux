#include <stdio.h>
#include "lux.h"
#include "window.h"
#include "flog.h"
#include <chrono>
#include <stdexcept>

class MainWindow : public lux::Window {
public:
    MainWindow() : lux::Window(lux::Point(1280, 720), "Hello Lux!", lux::Point(500, 300)) {

    }

private:
};

int main() {
    try {
        MainWindow mainWindow;
        mainWindow.show();
        lux::run();
    }
    catch (std::runtime_error e) {
        flog::error("ERROR: {}", e.what());
    }
    return 0;
}