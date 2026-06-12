#include <iostream>

#include <renderer.h>
#include <frame.h>
#include <camera.h>
#include <input.h>

int main() {

    // Initialization

    GLFWwindow* window = Renderer::instance().getWindow();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Input::init();
    glfwSetKeyCallback(window, Input::key_callback);

    FrameBuffer& frameBuffer = *Renderer::instance().frameBuffer;
    Camera& camera = Camera::instance();

    Renderer::instance().frameBuffer->clear(glm::vec3(0.1f));

    for (uint16_t i = 0; i < WIN_W /2; i++) {
        frameBuffer.setPixel(i, 100, glm::vec3(1.0f));
    }

    // ------------------------------------------------------------- //

    // Triangle t1 {
    //     math::vec2(0.0f, 1.0f),
    //     math::vec2(-1.0f, -1.0f),
    //     math::vec2(1.0f, -1.0f),
    //     math::vec3(1.0f, 1.0f, 0.0f)
    // };

    // Triangle t2 {
    //     math::vec2(0.5f, 0.8f),
    //     math::vec2(-0.6f, -0.3f),
    //     math::vec2(0.8f, -0.2f),
    //     math::vec3(1.0f, 1.0f, 1.0f)
    // };

    // Triangle3D t3 {

    //     math::vec3(-0.6f, -0.9f, 0.0f),
    //     math::vec3(-0.1f, 0.5f, 0.5f),
    //     math::vec3(0.7f, 0.4f, 1.0f)
    // };

    // Triangle3D t4 {
    //     math::vec3(0.0f, 1.0f, 0.4f),
    //     math::vec3(-1.0f, -1.0f, 0.3f),
    //     math::vec3(1.0f, -1.0f, 1.0f)
    // };

    // frameBuffer.drawTriangle(t1);
    // frameBuffer.drawTriangle(t2);

    // frameBuffer.fillTriangle(t1);
    // frameBuffer.gradientTriangle(t2);
    // frameBuffer.draw(t3, math::vec3(1.0f));
    // frameBuffer.draw(t4, math::vec3(1.0f, 0.5f, 0.8f));

    Triangle2D t1 {
        glm::vec2( 0.0f, 1.0f),
        glm::vec2(-1.0f,-1.0f),
        glm::vec2( 1.0f,-1.0f)
    };

    Triangle2D t2 {
        glm::vec2( 0.0f, 2.0f),
        glm::vec2(-1.0f,-1.0f),
        glm::vec2( 3.0f,-1.0f)
    };

    Triangle t3 {
        glm::vec4( 0.0f, 2.0f, 0.0f, 1.0f),
        glm::vec4(-1.0f,-1.0f, 0.0f, 1.0f),
        glm::vec4( 3.0f,-1.0f, 0.0f, 1.0f)
    };

    // t1.transform();
    // t2.transform();

    // frameBuffer.gradientTriangle(t1, glm::vec3(1.0f));
    // frameBuffer.gradientTriangle(t2, glm::vec3(1.0f));

    glm::mat4 squareModel (1.0f);
    squareModel = glm::scale(squareModel, glm::vec3(1.0f, 0.7f, 0.2f));

    glm::mat4 cubeModel (1.0f);
    cubeModel = glm::translate(cubeModel, glm::vec3(-3.0f, 0.0f, 1.0f));
    
    bool isRunning = true;
    float deltaTime, currentTime = 0.0f, lastTime = 0.0f;

    while (!glfwWindowShouldClose(window) && isRunning) {

        // Time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            isRunning = false;
        }

        camera.mouse_handler(window);
        camera.input_handler(window, deltaTime);
        Input::update();

        // Events
        glfwPollEvents();

        // Updates
        Renderer::instance().frameBuffer->update();
        camera.update(deltaTime);

        // Rendering
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        frameBuffer.clear();
        frameBuffer.draw(Shapes::instance().square, squareModel, glm::vec3(1.0f));
        frameBuffer.draw(Shapes::instance().cube,   cubeModel, glm::vec3(1.0f, 0.5f, 0.8f));

        Renderer::instance().render();

        glfwSwapBuffers(window);
    }

    Renderer::instance().terminate();

    return 0;
}