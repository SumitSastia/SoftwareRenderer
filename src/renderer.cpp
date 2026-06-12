#include <renderer.h>
#include <frame.h>

#include <string>

bool Renderer::culling = false;

bool Shader::init() {

    const char* vertSource = 
    "#version 450 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "layout (location = 1) in vec2 aTexCords;\n"
    "out vec2 vTexCords;\n"
    "void main() {\n"
        "gl_Position = vec4(aPos, 0.0, 1.0);\n"
        "vTexCords = aTexCords;\n"
    "}\0";

    const char* fragSource = 
    "#version 450 core\n"
    "in  vec2 vTexCords;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D screen;\n"
    "void main() {\n"
        "FragColor = vec4(vec3(texture(screen, vTexCords).rgb), 1.0);\n"
        "// FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
    "}\0";

    const unsigned int vertexShader   = glCreateShader(GL_VERTEX_SHADER);
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    int success;
    char infoLog[512];

    glShaderSource(vertexShader, 1, &vertSource, nullptr);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR: VERTEX-SHADER COMPILATION FAILED!\n" << infoLog << std::endl;
        return false;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR: FRAGMENT-SHADER COMPILATION FAILED!\n" << infoLog << std::endl;
        return false;
    }

    // Shader Program //
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR: SHADER-PROGRAM LINKING FAILED!\n" << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::use() const {
    glUseProgram(shaderProgram);
}

void Shader::setInt(const char* target, const int value) const {

    glUniform1i(
        glGetUniformLocation(shaderProgram, target),
        value
    );
}

// *********************************************************************************** //

bool Renderer::init() {

    if (!glfwInit()) {
        std::cerr << "Unable to initialize GLFW!" << std::endl;
        return false;
    }

    // OpenGL Configurations //
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);


    // ---------- Monitor --------------------- //

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    
    if(!monitor){
        std::cerr << "Unable to fetch the Monitor!" << std::endl;
        return false;
    }
    
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

    // ---------- Main Window ----------------- //

    window = glfwCreateWindow(WIN_W,WIN_H,"Snow-SoftwareRenderer 1.0", nullptr, nullptr);

    if(!window){
        std::cerr << "Unable to initialize the window!" << std::endl;
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGL()){
        std::cerr << "Unable to Load OpenGL!" << std::endl;
        return -1;
    }

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    const unsigned int win_Xpos = videoMode->width/2 - WIN_W/2;
    const unsigned int win_Ypos = videoMode->height/2 - WIN_H/2;

    glfwSetWindowPos(window,win_Xpos,win_Ypos);
    glfwSetWindowAttrib(window,GLFW_RESIZABLE,GLFW_FALSE);

    int frameWidth, frameHeight;
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    glViewport(0,0,frameWidth,frameHeight);
    
    glfwSwapInterval(1); // V-Sync

    // Max Textures that can be Loaded by GPU
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURES);

    // FrameBuffer
    frameBuffer = new FrameBuffer(
        static_cast<uint16_t>(WIN_W),
        static_cast<uint16_t>(WIN_H)
    );

    // Final Quad VAO Setup
    const float vertices[] = {

        // Position  // Cords
        -1.0f, 1.0f, 0.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 0.0f, 0.0f,

         1.0f, 1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f, 0.0f
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return true;
}

void Renderer::render() const {
    
    shader.use();
    shader.setInt("screen", 0);
    
    frameBuffer->bindBuffer();
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::terminate() {
    glfwDestroyWindow(window);
}

void Renderer::cullBackFace(const bool cull) {
    frameBuffer->cullBackFace = cull;
}