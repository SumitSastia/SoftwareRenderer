#pragma once

#include <cstdint>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Triangle2D {

    glm::vec2 v0;
    glm::vec2 v1;
    glm::vec2 v2;

    Triangle2D transform() const;
};

struct Triangle {

    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
};