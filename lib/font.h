#pragma once
#include <string>
#include "stb/stb_truetype.h"
#include "glad/glad.h"
#include "point.h"

namespace lux {
    class Font {
    public:
        Font(const std::string& path, int size);
        ~Font();

        stbtt_bakedchar* getCharInfo(int c);
        const Size& getBitmapSize();
        uint8_t* const getBitmap();

        int getHeight();
        int calcTextSize(const std::string& str);

    private:
        stbtt_bakedchar* bcs = NULL;
        uint8_t* bitmap = NULL;
        int height;
        Size bitmapSize;
    };
}