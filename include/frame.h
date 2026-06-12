#pragma once

#include <r_math.h>

class FrameBuffer {

    uint16_t width;
    uint16_t height;

    uint screenTexture;

    // uint32_t RGB_to_HEX(const math::vec3 color) const;
    // float checkEdge(const math::vec2& a, const math::vec2& b, const math::vec2& pixel);
    uint32_t RGB_to_HEX(const glm::vec3 color) const;
    float checkEdge(const glm::vec2& a, const glm::vec2& b, const glm::vec2& pixel);
    
public:
    
    FrameBuffer(const uint16_t width, const uint16_t height);

    uint32_t* colorBuffer;
    float*    depthBuffer;

    void update();

    // void setPixel(const u_int16_t x, const uint16_t y, const glm::vec3 color);
    // bool setDepth(const u_int16_t x, const uint16_t y, const float depth);

    void setPixel(const float x, const float y, const glm::vec3 color);
    bool setDepth(const float x, const float y, const float depth);

    void clear(const glm::vec3 color = glm::vec3(0.0f));

    void bindBuffer() const;

    // Geometry
    void drawLine(glm::vec2 a, glm::vec2 b, const glm::vec3 color);

    // @param triangle 3x glm::vec3 in Normalized Device Coordinates.
    void drawTriangle(const Triangle2D& triangle, const glm::vec3& color);

    // @param triangle 3x glm::vec3 in Normalized Device Coordinates.
    void fillTriangle(const Triangle2D& triangle, const glm::vec3& color);

    // @param triangle 3x glm::vec3 in Normalized Device Coordinates.
    void gradientTriangle(const Triangle2D& triangle, const glm::vec3& color);

    // 3D Geometry
    // @param triangle 3x glm::vec3 in Normalized Device Coordinates.
    void draw(const Triangle& triangle, const glm::vec3 color);

    void draw(const Shape& shape, const glm::vec3 color);
    void draw(const Shape& shape, const glm::mat4& model, const glm::vec3 color);
};