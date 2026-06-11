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