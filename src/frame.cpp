#include <frame.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

uint32_t FrameBuffer::RGB_to_HEX(const math::vec3 color) const {

    uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
    uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
    uint8_t b = static_cast<uint8_t>(color.b * 255.0f);

    // last 255 is alpha
    uint32_t hex = (r << 24) | (g << 16) | (b << 8) | 255;

    return hex;
}

void FrameBuffer::setPixel(const math::vec2 pixel, const math::vec3 color) {

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