#pragma once

#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

namespace colors {

    const glm::vec3 RED    { 1.000f, 0.000f, 0.000f };
    const glm::vec3 ORANGE { 0.882f, 0.100f, 0.118f };
    const glm::vec3 YELLOW { 1.000f, 1.000f, 0.000f };
    const glm::vec3 GREEN  { 0.000f, 1.000f, 0.000f };
    const glm::vec3 BLUE   { 0.000f, 0.000f, 1.000f };
    const glm::vec3 PINK   { 0.773f, 0.042f, 0.301f };
    const glm::vec3 WHITE  { 1.000f, 1.000f, 1.000f };
    const glm::vec3 BLACK  { 0.000f, 0.000f, 0.000f };
    const glm::vec3 GRAY   { 0.133f, 0.133f, 0.133f };
}