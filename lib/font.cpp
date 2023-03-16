#include "font.h"
#include "stb/stb_truetype.h"
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

        // Allocate bitmap
        // TODO: Try different sizes to not waste memory
        bitmap = new uint8_t[512*512];
        stbtt_bakedchar* bcs = new stbtt_bakedchar[256];
        stbtt_BakeFontBitmap(fontData, 0, size, bitmap, 512, 512, 0, 256, bcs);

        // free font data
        delete[] fontData;
    }

    Font::~Font() {
        if (bitmap) { delete[] bitmap; }
    }

    uint8_t* const Font::getBitmap() {
        return bitmap;
    }
}