#include <stdio.h>
#include "lux.h"
#include "window.h"
#include "flog.h"
#include "widgets/div.h"
#include "widgets/button.h"
#include <chrono>

class MainWindow : public lux::Window {
public:
    MainWindow() : lux::Window(lux::Point(320, 240), "Hello Lux!", lux::Point(500, 300)) {
        auto start = std::chrono::high_resolution_clock::now();

        btn = std::make_shared<lux::Button>(this, "Button 1");
        setRootWidget(btn);

        auto end = std::chrono::high_resolution_clock::now();
        flog::warn("Construct: {} us", (int)std::chrono::duration_cast<std::chrono::microseconds>(end-start).count());
    }

private:
    std::shared_ptr<lux::Button> btn;
};

int main() {
    MainWindow mainWindow;
    mainWindow.show();
    lux::run();
    return 0;
}