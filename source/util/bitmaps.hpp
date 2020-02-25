#ifndef UTILITY_BITMAPS_HPP
#define UTILITY_BITMAPS_HPP

#include <string>

struct Bitmap {
    int width;
    int height;
    const uint8_t *buffer;
};

const Bitmap GetBitmap(const std::string &key);

#endif