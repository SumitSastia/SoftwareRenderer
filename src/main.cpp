#include <iostream>

#include <renderer.h>

int main() {

    GLFWwindow* window = Renderer::instance().getWindow();

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    Renderer::instance().terminate();

    return 0;
}