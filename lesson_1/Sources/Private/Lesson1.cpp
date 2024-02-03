#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <string_view>

const std::string GetShaderContent(const std::string_view ShaderPath) {
    std::string ShaderContent;

    if (ShaderPath.empty()) {
        return ShaderContent;
    }

    std::ifstream File(ShaderPath.data());
    if (File.is_open()) {
        std::string FileLine;
        while (std::getline(File, FileLine)) {
            ShaderContent.append(FileLine);
            ShaderContent.append("\n");
        }

        ShaderContent.append("\0");
        File.close();
    }

    return ShaderContent;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == nullptr) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    } else {
        printf("GLFW Initialized\n");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    unsigned int VertexShader = -1, FragmentShader = -1;

    // Load and compile vertex shader
    {
        std::string ShaderString = GetShaderContent(VERTEX_SHADER_PATH);

        if (ShaderString.empty()) {
            printf("Filed to read shader by path %s", VERTEX_SHADER_PATH);
            glfwTerminate();
            return -1;
        }

        const char* ShaderContent = ShaderString.c_str();
        VertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VertexShader, 1, &ShaderContent, NULL);
        glCompileShader(VertexShader);
    }

    // Load and compile fragment shader
    {
        std::string ShaderString = GetShaderContent(FRAGMENT_SHADER_PATH);

        if (ShaderString.empty()) {
            printf("Filed to read shader by path %s", FRAGMENT_SHADER_PATH);
            glfwTerminate();
            return -1;
        }

        const char* ShaderContent = ShaderString.c_str();
        FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(FragmentShader, 1, &ShaderContent, NULL);
        glCompileShader(FragmentShader);
    }

    // Create Shader program
    unsigned int ShaderProgram;
    ShaderProgram = glCreateProgram();

    if (VertexShader < 0 || FragmentShader < 0) {
        printf("Filedt to create shaders. Vertex = %d, Fragment = %d\n", VertexShader, FragmentShader);
        glfwTerminate();
        return -1;
    }

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // Top X
        -1.0f, 0.5f, 0.0f, // left
        -0.5f, 0.5f, 0.0f, // right
        // Bottom X
        -1.0f, -0.5f, 0.0f, // left
        -0.5f, -0.5f, 0.0f, // right

        -0.75f,  0.0f, 0.0f,  // common top

        // Top B
        -0.5f, 0.5f, 0.0f, // left
        0.0f, 0.25f, 0.0f, // right

        // Bottom B
        0.0f, -0.25f, 0.0f, // right
        -0.5f,  -0.5f, 0.0f,  // top

        -0.5f, 0.0f, 0.0f, // Common B

        // Top P
        0.5f, 0.5f, 0.0f, // right

        // Bottom P
        0.0f, -0.5f, 0.0f, // bottom

        // Common
        0.0f, 0.5f, 0.0f, // left
        0.5f,  -0.5f, 0.0f,  // top
    };

    unsigned int indices [] = {
        0, 1, 4, // Top X
        2, 3, 4, // Bottom X
        5, 6, 9, // Top B
        7, 8, 9, // Bottom B
        10, 12, 13, // Top P
        11, 13, 12 // Bottom P
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ShaderProgram);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ShaderProgram);

    glfwTerminate();

    return 0;
}
