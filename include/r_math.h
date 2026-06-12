#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

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

    glm::vec4 v0;
    glm::vec4 v1;
    glm::vec4 v2;

    Triangle transform(const glm::mat4& model = glm::mat4(1.0f)) const;
};

struct Shape {

    std::vector <Triangle> triangles;
};  

struct Shapes {

    Shape square;
    Shape cube;

    Shapes();
    static Shapes& instance() {
        static Shapes instance {};
        return instance;
    }
};