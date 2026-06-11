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

Triangle Triangle::transform() const {

    const glm::mat4 mvp = Camera::instance().getPerspective() * Camera::instance().getView() * glm::mat4(1.0f);

    glm::vec4 gl_Pos0 = mvp * glm::vec4(v0.x, v0.y, v0.z, 1.0f);
    glm::vec4 gl_Pos1 = mvp * glm::vec4(v1.x, v1.y, v1.z, 1.0f);
    glm::vec4 gl_Pos2 = mvp * glm::vec4(v2.x, v2.y, v2.z, 1.0f);

    gl_Pos0.x /= gl_Pos0.w;
    gl_Pos1.x /= gl_Pos1.w;
    gl_Pos2.x /= gl_Pos2.w;

    gl_Pos0.y /= gl_Pos0.w;
    gl_Pos1.y /= gl_Pos1.w;
    gl_Pos2.y /= gl_Pos2.w;

    gl_Pos0.z /= gl_Pos0.w;
    gl_Pos1.z /= gl_Pos1.w;
    gl_Pos2.z /= gl_Pos2.w;

    return Triangle { gl_Pos0, gl_Pos1, gl_Pos2 };
}