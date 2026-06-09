#pragma once

#include <r_math.h>
#include <cstdint>

class FrameBuffer {

    uint16_t width;
    uint16_t height;

    uint screenTexture;

    FrameBuffer(const uint16_t width, const uint16_t height);

    uint32_t RGB_to_HEX(const math::vec3 color) const;

public:

    uint32_t* colorBuffer;

    void update();
    void setPixel(const math::vec2 pixel, const math::vec3 color);
    void clear(const math::vec3 color = math::vec3(0.0f));
};