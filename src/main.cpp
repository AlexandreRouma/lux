#include <stdio.h>
#include "lux.h"
#include "window.h"

int main() {
    lux::Window mainWindow(lux::Size(320, 240), "Hello Lux!", lux::Point(500, 300));
    lux::run();
    return 0;
}