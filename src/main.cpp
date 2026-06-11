#include <iostream>

#include <renderer.h>
#include <frame.h>

int main() {

    GLFWwindow* window = Renderer::instance().getWindow();

    FrameBuffer& frameBuffer = *Renderer::instance().frameBuffer;

    Renderer::instance().frameBuffer->clear(math::vec3(0.1f));

    for (uint16_t i = 0; i < WIN_W /2; i++) {
        Renderer::instance().frameBuffer->setPixel(math::vec2u(i,100), math::vec3(1.0f));
    }

    // ------------------------------------------------------------- //

    Triangle t1 {
        math::vec2(0.0f, 1.0f),
        math::vec2(-1.0f, -1.0f),
        math::vec2(1.0f, -1.0f),
        math::vec3(1.0f, 1.0f, 0.0f)
    };

     Triangle t2 {
        math::vec2(0.5f, 0.8f),
        math::vec2(-0.6f, -0.3f),
        math::vec2(0.8f, -0.2f),
        math::vec3(0.0f, 1.0f, 1.0f)
    };

    // frameBuffer.drawTriangle(t1);
    // frameBuffer.drawTriangle(t2);

    frameBuffer.fillTriangle(t1);
    frameBuffer.fillTriangle(t2);
    
    bool isRunning = true;

    while (!glfwWindowShouldClose(window) && isRunning) {

        // Inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            isRunning = false;
        }

        // Events
        glfwPollEvents();

        // Updates
        Renderer::instance().frameBuffer->update();

        // Rendering
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        Renderer::instance().render();

        glfwSwapBuffers(window);
    }

    Renderer::instance().terminate();

    return 0;
}