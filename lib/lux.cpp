#include "lux.h"
#include "backend.h"

namespace lux {
    GLuint fontTexId = -1;
    std::shared_ptr<Font> font;

    void run() {
        backend::run();
    }
}