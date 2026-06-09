#pragma once

#include <r_math.h>

class FrameBuffer {

    uint16_t width;
    uint16_t height;

    uint screenTexture;

    uint32_t RGB_to_HEX(const math::vec3 color) const;
    
public:
    
    FrameBuffer(const uint16_t width, const uint16_t height);

    uint32_t* colorBuffer;

    void update();
    void setPixel(const math::vec2u pixel, const math::vec3 color);
    void clear(const math::vec3 color = math::vec3(0.0f));

    void bindBuffer() const;

    // Geometry
    void drawLine(math::vec2 a, math::vec2 b);

    void drawTriangle(const Triangle& triangle);
    void fillTriangle(const Triangle& triangle);
};