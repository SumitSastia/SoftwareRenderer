#pragma once

typedef unsigned int uint;

namespace math {

    struct vec2 {
        float x;
        float y;
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
}