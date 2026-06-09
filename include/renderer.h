#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const unsigned int WIN_W = 1280;
const unsigned int WIN_H = 720;

class Renderer {

    GLFWmonitor* monitor;
    GLFWwindow*  window;

    int MAX_TEXTURES;
    static bool culling;

    Renderer() { 
        
        if (!init()) {
            std::cerr << "Unable to initialize the Renderer!" << std::endl;
        }
    }

public:

    static Renderer& instance() {
        
        static Renderer instance;
        return instance;
    }

    bool init(); /* Returns true if Renderer is successfully initialized. */
    void render() const {}
    void terminate();
    
    // ------------------------------ Global Render Functions ---------------------------- //

    static void enableCulling();
    static void disableCulling();
    static bool getCullingState() { return culling; }

    static void enableDepth();
    static void disableDepth();

    static void clear();

    GLFWwindow* getWindow() const { return window; }

};