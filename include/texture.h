#pragma once

#include <cstdint>

struct Texture {

    int width, height;
    int nrChannels;

    unsigned char* pixelData;
    uint32_t* color;

    Texture():
        width(0), height(0), nrChannels(0),
        pixelData(nullptr), color(nullptr) {
    }
    
    void load(const char* path);
    uint32_t sample(const float u, const float v) const;
};