#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

using namespace std;


unsigned int VAO, VBO, shaderProgram;


vector<float> DDA(float x1, float y1, float x2, float y2)
{
    vector<float> points;

    float dx = x2 - x1;
    float dy = y2 - y1;

    int steps = max(abs(dx), abs(dy));

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i <= steps; i++)
    {
        // Add the current point to the list
        points.push_back(x);
        points.push_back(y);
        points.push_back(0.0f); // z

        x += xInc;
        y += yInc;
    }

    return points;
}

// Resize callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// =========================
// SHADER SETUP
// =========================
void setupShader()
{
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main(){ gl_Position = vec4(aPos, 1.0); }";

    const char *fragmentShaderSource = "#version 330 core\n"
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


void setupBuffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void drawShape(float vertices[], int size, GLenum mode)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glUseProgram(shaderProgram);

    if (mode == GL_POINTS)
        glPointSize(10.0f);

    glDrawArrays(mode, 0, size / (3 * sizeof(float)));
}

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "Shapes Demossss", NULL, NULL);
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

    

   
    float x1_ndc = 2 * (100.0f / 800.0f) - 1.0f; // -0.75
    float y1_ndc = 1.0f - 2 * (100.0f / 600.0f); // 0.667
    float x2_ndc = 2 * (250.0f / 800.0f) - 1.0f; // -0.375
    float y2_ndc = 1.0f - 2 * (400.0f / 600.0f); // -0.333

    vector<float> ddaLinePoints = DDA(x1_ndc, y1_ndc, x2_ndc, y2_ndc);
    float *ddaLine = ddaLinePoints.data();
    int ddaLineSize = ddaLinePoints.size() * sizeof(float);

    float triangle[] = {
        -0.2f, -0.2f, 0.0f,
        0.2f, -0.2f, 0.0f,
        0.0f, 0.2f, 0.0f};

   

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw DDA line
        drawShape(ddaLine, ddaLineSize, GL_LINE_STRIP);

        // Draw triangle
        drawShape(triangle, sizeof(triangle), GL_TRIANGLES);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}