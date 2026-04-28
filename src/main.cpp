#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// =========================
// GLOBALS 
// =========================
unsigned int VAO, VBO, shaderProgram;

// Resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// =========================
// SHADER SETUP 
// =========================
void setupShader()
{
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main(){ gl_Position = vec4(aPos, 1.0); }";

    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){ FragColor = vec4(0.2f, 0.8f, 0.3f, 1.0f); }";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// =========================
// BUFFER SETUP 
// =========================
void setupBuffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// =========================
// SIMPLE DRAW FUNCTION
// =========================
void drawShape(float vertices[], int size, GLenum mode)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glUseProgram(shaderProgram);

    if(mode == GL_POINTS)
        glPointSize(10.0f);

    glDrawArrays(mode, 0, size / (3 * sizeof(float)));
}

// =========================
// MAIN
// =========================
int main()
{
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Shapes Demossss", NULL, NULL);
    if (!window)
    {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Setup hidden parts
    setupShader();
    setupBuffer();

    // =========================
    // SHAPES 
    // =========================

   float point[] = {
    -0.8f, 0.8f, 0.0f
};

float line[] = {
    -0.5f, 0.5f, 0.0f,
    -0.2f, 0.5f, 0.0f
};

float triangle[] = {
     0.0f,  0.5f, 0.0f,
     0.3f,  0.5f, 0.0f,
     0.15f, 0.8f, 0.0f
};

float rectangle[] = {
    -0.8f, -0.2f, 0.0f,
    -0.4f, -0.2f, 0.0f,
    -0.4f, -0.6f, 0.0f,

    -0.4f, -0.6f, 0.0f,
    -0.8f, -0.6f, 0.0f,
    -0.8f, -0.2f, 0.0f
};

float pentagon[] = {
     0.4f,  0.2f, 0.0f,
     0.2f, -0.1f, 0.0f,
     0.3f, -0.5f, 0.0f,
     0.6f, -0.5f, 0.0f,
     0.7f, -0.1f, 0.0f
};

    // =========================
    // RENDER LOOP
    // =========================

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // CHANGE THIS LINE DURING CLASS 🔥

      drawShape(point, sizeof(point), GL_POINTS);
drawShape(line, sizeof(line), GL_LINES);
drawShape(triangle, sizeof(triangle), GL_TRIANGLES);
drawShape(rectangle, sizeof(rectangle), GL_TRIANGLES);
// drawShape(pentagon, sizeof(pentagon), GL_TRIANGLE_FAN);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}