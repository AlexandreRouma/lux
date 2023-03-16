#pragma once
#include <string>

namespace lux {
    class Font {
    public:
        Font(const std::string& path, int size);
        ~Font();

        uint8_t* const getBitmap();

    private:
        uint8_t* bitmap = NULL;
    };
}