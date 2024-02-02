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
        -0.6f, 0.0f, 0.0f, // left
        -0.2f, 0.0f, 0.0f, // right
        -0.4f,  0.5f, 0.0f,  // top
        //
        0.2f, 0.0f, 0.0f, // right
        0.6f, 0.0f, 0.0f, // left
        0.4f,  0.5f, 0.0f  // top

    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(ShaderProgram);

    glfwTerminate();

    return 0;
}
