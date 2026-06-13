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

struct Vertex {

    glm::vec4 vertex;
    glm::vec2 uv;

    Vertex() = default;

    Vertex(const glm::vec4& vertex):
        vertex(vertex), uv(glm::vec2(1.0f)) {
    }

    Vertex(const glm::vec4& vertex, const glm::vec2& uv):
        vertex(vertex), uv(uv) {
    }

    operator const glm::vec4&() const { return vertex; }
};

struct Triangle {

    Vertex v0;
    Vertex v1;
    Vertex v2;

    Triangle transform(const glm::mat4& model = glm::mat4(1.0f)) const;
};

struct Shape {

    std::vector <Triangle> triangles;
    
    Vertex intersect(const Vertex& a, const Vertex& b) const;
    std::vector <Triangle> transform(const glm::mat4& model = glm::mat4(1.0f)) const;
    
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