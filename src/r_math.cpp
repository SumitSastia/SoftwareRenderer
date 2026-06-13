#include <r_math.h>
#include <camera.h>

Triangle2D Triangle2D::transform() const {

    const glm::mat4 mvp = Camera::instance().getPerspective() * Camera::instance().getView() * glm::mat4(1.0f);

    glm::vec4 gl_Position0 = mvp * glm::vec4(v0.x, v0.y, 0.0f, 1.0f);
    glm::vec4 gl_Position1 = mvp * glm::vec4(v1.x, v1.y, 0.0f, 1.0f);
    glm::vec4 gl_Position2 = mvp * glm::vec4(v2.x, v2.y, 0.0f, 1.0f);

    gl_Position0 /= gl_Position0.w;
    gl_Position1 /= gl_Position1.w;
    gl_Position2 /= gl_Position2.w;

    glm::vec2 nv0 = glm::vec2(gl_Position0);
    glm::vec2 nv1 = glm::vec2(gl_Position1);
    glm::vec2 nv2 = glm::vec2(gl_Position2);

    return Triangle2D { nv0, nv1, nv2 };
}

Triangle Triangle::transform(const glm::mat4& model) const {

    const glm::mat4 mvp = Camera::instance().getPerspective() * Camera::instance().getView() * model;

    glm::vec4 gl_Pos0 = mvp * glm::vec4(v0.vertex.x, v0.vertex.y, v0.vertex.z, 1.0f);
    glm::vec4 gl_Pos1 = mvp * glm::vec4(v1.vertex.x, v1.vertex.y, v1.vertex.z, 1.0f);
    glm::vec4 gl_Pos2 = mvp * glm::vec4(v2.vertex.x, v2.vertex.y, v2.vertex.z, 1.0f);

    gl_Pos0.x /= gl_Pos0.w;
    gl_Pos1.x /= gl_Pos1.w;
    gl_Pos2.x /= gl_Pos2.w;

    gl_Pos0.y /= gl_Pos0.w;
    gl_Pos1.y /= gl_Pos1.w;
    gl_Pos2.y /= gl_Pos2.w;

    gl_Pos0.z /= gl_Pos0.w;
    gl_Pos1.z /= gl_Pos1.w;
    gl_Pos2.z /= gl_Pos2.w;

    return Triangle {
        Vertex { gl_Pos0, v0.uv },
        Vertex { gl_Pos1, v1.uv },
        Vertex { gl_Pos2, v2.uv }
    };
}

Vertex Shape::intersect(const Vertex&a, const Vertex& b) const {

    float da = a.vertex.z + a.vertex.w;
    float db = b.vertex.z + b.vertex.w;

    float t = da / (da - db);

    Vertex result;

    result.vertex =
        a.vertex +
        t * (b.vertex - a.vertex);

    result.uv =
        a.uv +
        t * (b.uv - a.uv);

    return result;
}

std::vector <Triangle> Shape::transform(const glm::mat4& model) const {

    std::vector <Triangle> transformedTris;
    const glm::mat4 mvp = Camera::instance().getPerspective() * Camera::instance().getView() * model;

    for (const Triangle triangle : triangles) {

        Vertex v0 = triangle.v0;
        Vertex v1 = triangle.v1;
        Vertex v2 = triangle.v2;

        v0.vertex = mvp * glm::vec4(v0.vertex.x, v0.vertex.y, v0.vertex.z, 1.0f);
        v1.vertex = mvp * glm::vec4(v1.vertex.x, v1.vertex.y, v1.vertex.z, 1.0f);
        v2.vertex = mvp * glm::vec4(v2.vertex.x, v2.vertex.y, v2.vertex.z, 1.0f);

        bool i0 = (v0.vertex.w > 0.0f);
        bool i1 = (v1.vertex.w > 0.0f);
        bool i2 = (v2.vertex.w > 0.0f);

        int8_t total_inside = (int)i0 + (int)i1 + (int)i2;

        if (total_inside == 3) {

            v0.vertex.x /= v0.vertex.w;
            v1.vertex.x /= v1.vertex.w;
            v2.vertex.x /= v2.vertex.w;
            
            v0.vertex.y /= v0.vertex.w;
            v1.vertex.y /= v1.vertex.w;
            v2.vertex.y /= v2.vertex.w;
            
            v0.vertex.z /= v0.vertex.w;
            v1.vertex.z /= v1.vertex.w;
            v2.vertex.z /= v2.vertex.w;

            transformedTris.push_back({ v0, v1, v2 });
        }
        else if (total_inside == 2) {

            Vertex n0, n1, in0, in1;

            if (i1 && i2) {

                n0 = v1;
                n1 = v2;
                in0 = intersect(v1, v0);
                in1 = intersect(v2, v0);
            }
            else if (i0 && i2) {

                n0 = v0;
                n1 = v2;
                in0 = intersect(v0, v1);
                in1 = intersect(v2, v1);
            }
            else if (i0 && i1) {

                n0 = v0;
                n1 = v1;
                in0 = intersect(v0, v2);
                in1 = intersect(v1, v2);
            }

            n0.vertex.x  /= n0.vertex.w;
            n1.vertex.x  /= n1.vertex.w;
            in0.vertex.x /= in0.vertex.w;
            in1.vertex.x /= in1.vertex.w;
            
            n0.vertex.y  /= n0.vertex.w;
            n1.vertex.y  /= n1.vertex.w;
            in0.vertex.y /= in0.vertex.w;
            in1.vertex.y /= in1.vertex.w;
            
            n0.vertex.z  /= n0.vertex.w;
            n1.vertex.z  /= n1.vertex.w;
            in0.vertex.z /= in0.vertex.w;
            in1.vertex.z /= in1.vertex.w;

            transformedTris.push_back({ n0, n1, in0 });
            transformedTris.push_back({ n1, in1, in0 });

        }
        else if (total_inside == 1) {

            Vertex n0, in0, in1;

            if (i0) {
                n0  = v0;
                in0 = intersect(v0, v1);
                in1 = intersect(v0, v2);
            }
            else if (i1) {
                n0  = v1;
                in0 = intersect(v1, v0);
                in1 = intersect(v1, v2);
            }
            else if (i2) {
                n0  = v2;
                in0 = intersect(v2, v0);
                in1 = intersect(v2, v1);
            }

            n0.vertex.x  /= n0.vertex.w;
            in0.vertex.x /= in0.vertex.w;
            in1.vertex.x /= in1.vertex.w;
            
            n0.vertex.y  /= n0.vertex.w;
            in0.vertex.y /= in0.vertex.w;
            in1.vertex.y /= in1.vertex.w;
            
            n0.vertex.z  /= n0.vertex.w;
            in0.vertex.z /= in0.vertex.w;
            in1.vertex.z /= in1.vertex.w;

            transformedTris.push_back({ n0, in0, in1 });
        }
        else {
            continue;
        }
    }

    return transformedTris;
}

Shapes::Shapes() {

    square.triangles = {
        
        Triangle {
            Vertex { glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
            Vertex { glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
            Vertex { glm::vec4( 1.0f,-1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) }
        },
        Triangle {
            Vertex { glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
            Vertex { glm::vec4( 1.0f,-1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
            Vertex { glm::vec4(-1.0f,-1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) }
        }
    };

    cube.triangles = {

        // Front
        Triangle {
            glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4( 1.0f,-1.0f, 1.0f, 1.0f)
        },
        Triangle {
            glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4( 1.0f,-1.0f, 1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f)
        },

        // Back
        Triangle {
            glm::vec4( 1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f)
        },
        Triangle {
            glm::vec4( 1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f),
            glm::vec4( 1.0f,-1.0f,-1.0f, 1.0f)
        },

        // Left
        Triangle {
            glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f)
        },
        Triangle {
            glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f)
        },

        // Right
        Triangle {
            glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4( 1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4( 1.0f,-1.0f,-1.0f, 1.0f)
        },
        Triangle {
            glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4( 1.0f,-1.0f,-1.0f, 1.0f),
            glm::vec4( 1.0f,-1.0f, 1.0f, 1.0f)
        },

        // Top
        Triangle {
            glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4( 1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f)
        },
        Triangle {
            glm::vec4(-1.0f, 1.0f,-1.0f, 1.0f),
            glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f)
        },

        // Down
        Triangle {
            glm::vec4( 1.0f,-1.0f,-1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f,-1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f)
        },
        Triangle {
            glm::vec4( 1.0f,-1.0f,-1.0f, 1.0f),
            glm::vec4(-1.0f,-1.0f, 1.0f, 1.0f),
            glm::vec4( 1.0f,-1.0f, 1.0f, 1.0f)
        }
    };

}