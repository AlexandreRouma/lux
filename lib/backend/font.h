#pragma once
#include <string>
#include "stb/stb_truetype.h"
#include "glad/glad.h"

namespace lux {
    class Font {
    public:
        Font(const std::string& path, int size);
        ~Font();

        stbtt_bakedchar* getCharInfo(int c);
        uint8_t* const getBitmap();

        int calcTextSize(const std::string& str);

        GLuint texId;

    private:
        stbtt_bakedchar* bcs = NULL;
        uint8_t* bitmap = NULL;
    };
}