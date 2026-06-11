#pragma once
#include <cstdint>
#include <iostream>

typedef unsigned int uint;

namespace math {

    struct vec2 {

        float x;
        float y;

        math::vec2 convertRange() const;

        vec2 operator+(const float scalar) const {
            return vec2(scalar + x, scalar + y);
        }

        vec2 operator*(const float scalar) const {
            return vec2(scalar * x, scalar * y);
        }
    };

    struct vec2u {

        uint16_t x;
        uint16_t y;

        vec2u(uint16_t x, uint16_t y):
            x(x), y(y) {
        }

        vec2u(const vec2 vec):
            x(static_cast<uint16_t>(vec.x)),
            y(static_cast<uint16_t>(vec.y)) {
        }

        vec2 operator+(const float scalar) const {
            return vec2(scalar + x, scalar + y);
        }

        vec2 operator*(const float scalar) const {
            return vec2(scalar * x, scalar * y);
        }
    };

    struct vec3 {

        union {
            struct { float x,y,z; };
            struct { float r,g,b; };
        };

        vec3(float n):
            x(n), y(n), z(n) {
        }

        vec3(float x, float y , float z):
            x(x), y(y), z(z) {
        }
    };

    struct vec4 {
        float r;
        float g;
        float b;
        float a;
    };

    struct mat3 {
        vec3 matrix[3];
    };

    struct mat4 {
        vec3 matrix[4];
    };

    inline float dot(vec2 a, vec2 b) {
        return a.x * b.x + a.y * b.y;
    }

    inline vec2 perpendicular(vec2 vec) {
        return vec2(vec.y, -vec.x);
    }

    inline void swap(uint16_t& x, uint16_t& y) {
        uint16_t temp = x;
        x = y;
        y = temp;
    }

    inline void printVec2(const vec2u vec) {
        std::cout << "x: " << vec.x << std::endl;
        std::cout << "y: " << vec.y << std::endl;
    }
}

struct Triangle {

    math::vec2 a;
    math::vec2 b;
    math::vec2 c;
    math::vec3 color;
};