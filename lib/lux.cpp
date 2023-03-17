#include "lux.h"
#include "backend.h"

namespace lux {
    std::shared_ptr<Font> font;
    
    void run() {
        backend::run();
    }
}