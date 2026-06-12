#include <iostream>

#include <renderer.h>
#include <frame.h>
#include <camera.h>
#include <input.h>
#include <texture.h>

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

    glm::mat4 squareModel (1.0f);
    // squareModel = glm::scale(squareModel, glm::vec3(1.0f, 0.7f, 0.2f));

    glm::mat4 cubeModel (1.0f);
    cubeModel = glm::translate(cubeModel, glm::vec3(-3.0f, 0.0f, 1.0f));

    Texture tex_woodbox;
    tex_woodbox.load("assets/wood_box.png");
    
    bool isRunning = true;
    float deltaTime, currentTime = 0.0f, lastTime = 0.0f;

    while (!glfwWindowShouldClose(window) && isRunning) {

        // Time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Inputs
        if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
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
        // frameBuffer.draw(Shapes::instance().square, squareModel, glm::vec3(1.0f));
        frameBuffer.draw(Shapes::instance().cube,   cubeModel, glm::vec3(1.0f));
        // frameBuffer.drawWireframe(Shapes::instance().cube,   cubeModel, glm::vec3(1.0f));

        frameBuffer.drawTexture(Shapes::instance().square, squareModel, tex_woodbox);

        Renderer::instance().render();

        glfwSwapBuffers(window);
    }

    Renderer::instance().terminate();

    return 0;
}