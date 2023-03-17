#define STB_TRUETYPE_IMPLEMENTATION
#include "font.h"
#include <fstream>
#include <stdexcept>


namespace lux {
    Font::Font(const std::string& path, int size) {
        // Open font file
        std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
        if (!file.is_open()) { throw std::runtime_error("Could not open font file"); }

        // Save length and seek to beginning
        int fontLen = file.tellg();
        file.seekg(file.beg);

        // Read in font data
        uint8_t* fontData = new uint8_t[fontLen];
        file.read((char*)fontData, fontLen);
        file.close();

        // Allocate bitmap
        // TODO: Try different sizes to not waste memory
        bitmap = new uint8_t[512*512];
        bcs = new stbtt_bakedchar[256];
        stbtt_BakeFontBitmap(fontData, 0, size, bitmap, 512, 512, 0, 256, bcs);

        // Allocate and write texture
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

        // free font data
        delete[] fontData;
    }

    Font::~Font() {
        if (bitmap) { delete[] bitmap; }
        if (bcs) { delete[] bcs; }
    }

    stbtt_bakedchar* Font::getCharInfo(int c) {
        return &bcs[c];
    }

    uint8_t* const Font::getBitmap() {
        return bitmap;
    }

    int Font::calcTextSize(const std::string& str) {
        float len = 0;
        for (char c : str) {
            len += bcs[c].xadvance;
        }
        return ceil(len);
    }
}