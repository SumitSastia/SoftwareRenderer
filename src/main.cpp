#include <iostream>

#include <renderer.h>
#include <frame.h>

int main() {

    GLFWwindow* window = Renderer::instance().getWindow();

    Renderer::instance().frameBuffer->clear(math::vec3(0.8f));

    for (uint16_t i = 0; i < WIN_W; i++) {
        Renderer::instance().frameBuffer->setPixel(math::vec2u(i,10), math::vec3(0.0f));
    }
    
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