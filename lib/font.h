#pragma once
#include <string>
#include "stb/stb_truetype.h"
#include "glad/glad.h"
#include "vec2.h"

namespace lux {
    class Font {
    public:
        Font(const std::string& path, int size);
        ~Font();

        stbtt_bakedchar* getCharInfo(int c);
        const Size& getBitmapSize();
        uint8_t* const getBitmap();

        int getHeight();
        float calcTextWidth(const std::string& str);

        stbtt_bakedchar* bcs = NULL;

        float ascent, descent, lineGap;

    private:
        uint8_t* fontData = NULL;
        uint8_t* bitmap = NULL;
        stbtt_fontinfo fi;
        int height;
        Size bitmapSize;
    };
}