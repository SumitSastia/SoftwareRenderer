#include <iostream>

#include <renderer.h>
#include <frame.h>

int main() {

    GLFWwindow* window = Renderer::instance().getWindow();

    Renderer::instance().frameBuffer->clear(math::vec3(1.0f));
    Renderer::instance().frameBuffer->update();

    bool isRunning = true;

    while (!glfwWindowShouldClose(window) && isRunning) {

        // Inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            isRunning = false;
        }

        // Events
        glfwPollEvents();

        // Rendering
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer::instance().render();

        glfwSwapBuffers(window);
    }

    Renderer::instance().terminate();

    return 0;
}