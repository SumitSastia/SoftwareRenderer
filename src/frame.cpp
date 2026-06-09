#include <frame.h>
#include <renderer.h>

FrameBuffer::FrameBuffer(const uint16_t width, const uint16_t height):
    width(width), height(height)
{
    colorBuffer = new uint32_t[width * height]();

    for (uint32_t i = 0; i < width*height; i++) {
        colorBuffer[i] = 0xFF0000FF;
    }

    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::update() {
    
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer);
}

#include <iostream>

uint32_t FrameBuffer::RGB_to_HEX(const math::vec3 color) const {

    uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
    uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
    uint8_t b = static_cast<uint8_t>(color.b * 255.0f);

    // last 255 is alpha
    // uint32_t hex = (r << 24) | (g << 16) | (b << 8) | 0xFF;
    uint32_t hex = (0xFF << 24) | (b << 16) | (g << 8) | r;

    return hex;
}

void FrameBuffer::setPixel(const math::vec2u pixel, const math::vec3 color) {

    if (pixel.x < 0 || pixel.x > WIN_W) return;
    if (pixel.y < 0 || pixel.y > WIN_H) return;

    uint32_t index = pixel.y * width + pixel.x;
    colorBuffer[index] = RGB_to_HEX(color);
}

void FrameBuffer::clear(const math::vec3 color) {

    uint32_t totalPixels = width * height;

    for (uint32_t i = 0; i < totalPixels; i++) {
        colorBuffer[i] = RGB_to_HEX(color);
    }
}

void FrameBuffer::bindBuffer() const {

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
}

void FrameBuffer::drawLine(math::vec2 a, math::vec2 b) {

    // Range conversion: [-1, 1] -> [0, 1]
    a = a * 0.5f + 0.5f;
    b = b * 0.5f + 0.5f;

    a.x *= WIN_W;
    b.x *= WIN_W;

    a.y *= WIN_H;
    b.y *= WIN_H;
    
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    
    float steps = std::max(abs(dx), abs(dy));

    dx /= steps;
    dy /= steps;

    for (uint16_t i = 0; i < steps; i++) {

        setPixel(math::vec2u(a), math::vec3(0.0f, 0.0f, 1.0f));

        a.x += dx;
        a.y += dy;
    }
}