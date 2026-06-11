#include <frame.h>
#include <renderer.h>
#include <camera.h>

FrameBuffer::FrameBuffer(const uint16_t width, const uint16_t height):
    width(width), height(height)
{
    colorBuffer = new uint32_t[width * height];
    depthBuffer = new float[width * height];

    for (uint32_t i = 0; i < width * height; i++) {
        colorBuffer[i] = 0xFF0000FF;
        depthBuffer[i] = 1.0f;
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

uint32_t FrameBuffer::RGB_to_HEX(const glm::vec3 color) const {

    uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
    uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
    uint8_t b = static_cast<uint8_t>(color.b * 255.0f);

    // last 255 is alpha
    // uint32_t hex = (r << 24) | (g << 16) | (b << 8) | 0xFF;
    uint32_t hex = (0xFF << 24) | (b << 16) | (g << 8) | r;

    return hex;
}

void FrameBuffer::setPixel(const float x, const float y, const glm::vec3 color) {

    if (x < 0 || x > WIN_W) return;
    if (y < 0 || y > WIN_H) return;

    uint32_t index = static_cast<uint16_t>(y) * width + static_cast<uint16_t>(x);
    colorBuffer[index] = RGB_to_HEX(color);
}

bool FrameBuffer::setDepth(const float x, const float y, const float depth) {

    if (x < 0 || x > WIN_W) return false;
    if (y < 0 || y > WIN_H) return false;

    uint32_t index = static_cast<uint16_t>(y) * width + static_cast<uint16_t>(x);
    
    if (depth < depthBuffer[index]) {
        depthBuffer[index] = depth;
        return true;
    }

    return false;
}

void FrameBuffer::clear(const glm::vec3 color) {

    uint32_t totalPixels = width * height;

    for (uint32_t i = 0; i < totalPixels; i++) {
        colorBuffer[i] = RGB_to_HEX(color);
        depthBuffer[i] = 1.0f;
    }
}

void FrameBuffer::bindBuffer() const {

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
}

void FrameBuffer::drawLine(glm::vec2 a, glm::vec2 b, const glm::vec3 color) {

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

        setPixel(a.x, a.y, color);

        a.x += dx;
        a.y += dy;
    }
}

void FrameBuffer::drawTriangle(const Triangle2D& triangle, const glm::vec3& color) {

    drawLine(triangle.v0, triangle.v1, color);
    drawLine(triangle.v1, triangle.v2, color);
    drawLine(triangle.v2, triangle.v0, color);
}

glm::vec2 convertRange(const glm::vec2& vec) {

    glm::vec2 new_vec;

    new_vec.x = (vec.x * 0.5 + 0.5) * WIN_W;
    new_vec.y = (vec.y * 0.5 + 0.5) * WIN_H;

    return new_vec;
}

glm::vec3 convertRange(const glm::vec3& vec) {

    glm::vec3 new_vec;

    new_vec.x = (vec.x * 0.5 + 0.5) * WIN_W;
    new_vec.y = (vec.y * 0.5 + 0.5) * WIN_H;
    new_vec.z = vec.z;

    return new_vec;
}

float FrameBuffer::checkEdge(const glm::vec2& a, const glm::vec2& b, const glm::vec2& pixel) {
    return (pixel.x - a.x) * (b.y - a.y) - (pixel.y - a.y) * (b.x - a.x);
}

void FrameBuffer::fillTriangle(const Triangle2D& triangle, const glm::vec3& color) {

    glm::vec2 v0 = triangle.v0;
    glm::vec2 v1 = triangle.v1;
    glm::vec2 v2 = triangle.v2;

    // Bounding Box
    float minX = (std::min(std::min(v0.x, v1.x), v2.x) * 0.5 + 0.5) * WIN_W;
    float maxX = (std::max(std::max(v0.x, v1.x), v2.x) * 0.5 + 0.5) * WIN_W;

    float minY = (std::min(std::min(v0.y, v1.y), v2.y) * 0.5 + 0.5) * WIN_H;
    float maxY = (std::max(std::max(v0.y, v1.y), v2.y) * 0.5 + 0.5) * WIN_H;

    v0 = convertRange(v0);
    v1 = convertRange(v1);
    v2 = convertRange(v2);

    for (float i = minX; i <= maxX; i++) {
        for (float j = minY; j <= maxY; j++) {

            const glm::vec2 pixel(i, j);

            if (
                (checkEdge(v0, v1, pixel) >= 0 &&
                 checkEdge(v1, v2, pixel) >= 0 &&
                 checkEdge(v2, v0, pixel) >= 0) ||
                (checkEdge(v0, v1, pixel) <= 0 &&
                 checkEdge(v1, v2, pixel) <= 0 &&
                 checkEdge(v2, v0, pixel) <= 0)
            ) {
                setPixel(i, j, color);
            }
        }
    }
}

void FrameBuffer::gradientTriangle(const Triangle2D& triangle, const glm::vec3& color) {

    Triangle2D tri = triangle.transform();

    glm::vec2 v0 = tri.v0;
    glm::vec2 v1 = tri.v1;
    glm::vec2 v2 = tri.v2;

    // Bounding Box
    float minX = (std::min(std::min(v0.x, v1.x), v2.x) * 0.5 + 0.5) * WIN_W;
    float maxX = (std::max(std::max(v0.x, v1.x), v2.x) * 0.5 + 0.5) * WIN_W;

    float minY = (std::min(std::min(v0.y, v1.y), v2.y) * 0.5 + 0.5) * WIN_H;
    float maxY = (std::max(std::max(v0.y, v1.y), v2.y) * 0.5 + 0.5) * WIN_H;

    v0 = convertRange(v0);
    v1 = convertRange(v1);
    v2 = convertRange(v2);

    for (float i = minX; i <= maxX; i++) {
        for (float j = minY; j <= maxY; j++) {

            const glm::vec2 pixel(i, j);

            if (
                (checkEdge(v0, v1, pixel) >= 0  &&
                 checkEdge(v1, v2, pixel) >= 0  &&
                 checkEdge(v2, v0, pixel) >= 0) ||
                (checkEdge(v0, v1, pixel) <= 0  &&
                 checkEdge(v1, v2, pixel) <= 0  &&
                 checkEdge(v2, v0, pixel) <= 0)
            ) {
                // Gradient Triangle
                float area = checkEdge(v0, v1, v2);

                float alpha = checkEdge(v1, v2, pixel) / area;
                float beta  = checkEdge(v2, v0, pixel) / area;
                float gamma = checkEdge(v0, v1, pixel) / area;

                glm::vec3 new_color {
                    alpha * color.r,
                    beta  * color.g,
                    gamma * color.b
                };

                setPixel(i, j, new_color);
            }
        }
    }
}

void FrameBuffer::draw(const Triangle& triangle, const glm::vec3 color) {

    glm::vec2 v0 = triangle.v0;
    glm::vec2 v1 = triangle.v1;
    glm::vec2 v2 = triangle.v2;

    // Bounding Box
    float minX = (std::min(std::min(v0.x, v1.x), v2.x) * 0.5 + 0.5) * WIN_W;
    float maxX = (std::max(std::max(v0.x, v1.x), v2.x) * 0.5 + 0.5) * WIN_W;

    float minY = (std::min(std::min(v0.y, v1.y), v2.y) * 0.5 + 0.5) * WIN_H;
    float maxY = (std::max(std::max(v0.y, v1.y), v2.y) * 0.5 + 0.5) * WIN_H;

    v0 = convertRange(v0);
    v1 = convertRange(v1);
    v2 = convertRange(v2);

    for (float i = minX; i <= maxX; i++) {
        for (float j = minY; j <= maxY; j++) {

            const glm::vec2 pixel(i, j);

            if (
                (checkEdge(v0, v1, pixel) >= 0  &&
                 checkEdge(v1, v2, pixel) >= 0  &&
                 checkEdge(v2, v0, pixel) >= 0) ||
                (checkEdge(v0, v1, pixel) <= 0  &&
                 checkEdge(v1, v2, pixel) <= 0  &&
                 checkEdge(v2, v0, pixel) <= 0)
            ) {
                // Gradient Triangle
                float area = checkEdge(v0, v1, v2);

                float alpha = checkEdge(v1, v2, pixel) / area;
                float beta  = checkEdge(v2, v0, pixel) / area;
                float gamma = checkEdge(v0, v1, pixel) / area;

                glm::vec3 new_color {
                    alpha * color.r,
                    beta  * color.g,
                    gamma * color.b
                };

                float depth = {
                    alpha * triangle.v0.z +
                    beta  * triangle.v1.z +
                    gamma * triangle.v2.z 
                };

                if (setDepth(i, j, depth)) {
                    // setPixel(pixel, math::vec3(1.0f - depth));
                    setPixel(i, j, new_color);
                }
            }
        }
    }
}