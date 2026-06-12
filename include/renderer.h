#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <r_math.h>

class FrameBuffer;

const unsigned int WIN_W = 1280;
const unsigned int WIN_H = 720;

class Shader {

    unsigned int shaderProgram;
  
public:

    bool init();
    void use() const;
    void setInt(const char* target, const int data) const;
};

class Renderer {

    GLFWmonitor* monitor;
    GLFWwindow*  window;

    Shader shader;

    // FinalRender Data
    unsigned int VAO, VBO;
    
    int MAX_TEXTURES;
    static bool culling;
    
    Renderer() {
        
        if (!init()) {
            std::cerr << "ERROR::Unable to initialize the Renderer!" << std::endl;
        }

        if (!shader.init()) {
            std::cerr << "ERROR::Unable to initialize the Shader!" << std::endl;
        }
    }
    
public:

    FrameBuffer* frameBuffer;
    std::vector <Triangle> renderQueue;

    static Renderer& instance() {
        
        static Renderer instance;
        return instance;
    }

    bool init();
    void render() const;
    void terminate();

    void cullBackFace(const bool cull);

    // Triggers the instance
    void trigger() {}

    GLFWwindow* getWindow() const { return window; }

};