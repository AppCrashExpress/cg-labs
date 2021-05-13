#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <cmath>

#include "shader.h"
#include "line.h"
#include "graph.h"
#include "text.h"
#include "input.h"

namespace {
    constexpr int    SCR_WIDTH  = 600;
    constexpr int    SCR_HEIGHT = 600;

    constexpr size_t TICK_COUNT = 9;

    GLfloat scale  = 0.9f;
    Coords  offset = { 0.0f, 0.0f };
    Coords  scrOffset = { 0, 0 };
}

void manageInput() {
    constexpr float moveSpeed = 0.003f;
    constexpr float scaleSpeed = 0.003f;

    offset.x += Input::leftPressed * moveSpeed -
        Input::rightPressed * moveSpeed;
    offset.y += Input::downPressed * moveSpeed -
        Input::upPressed * moveSpeed;

    scale += Input::zoomInPressed * scaleSpeed -
        Input::zoomOutPressed * scaleSpeed;
}

void adjustViewport(GLFWwindow* window, const GLsizei width, const GLsizei height) {
    float dWidth = static_cast<float>(static_cast<int>(width));
    float dHeight = static_cast<float>(static_cast<int>(height));

    GLsizei max = 0;
    if (width > height) {
        max = width;
        scrOffset = { 0, (dWidth - dHeight) / (dWidth) };
    }
    else {
        max = height;
        scrOffset = { (dHeight - dWidth) / (dHeight), 0 };
    }

    glViewport(0, 0, max, max);
};

int main() {
    
    int lineConstant;
    float step;
    std::cout << "Formula constant 'a': ";
    std::cin >> lineConstant;
    std::cout << "Step: ";
    std::cin >> step;
    if (!std::cin.good()) {
        std::cerr << "Input stream failure\n";
        return -1;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Polar coordinates graph", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, adjustViewport);
    glfwSetKeyCallback(window, Input::keyboardCallback);

    if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) ) {
        std::cerr << "GLAD failed to initialize" << std::endl;
        glfwTerminate();
        return -1;
    }

    // VAO and VBO
    unsigned int graphVAO, graphVBO;
    unsigned int lineVAO, lineVBO;

    glGenVertexArrays(1, &lineVAO);
    glGenVertexArrays(1, &graphVAO);
    glGenBuffers(1, &lineVBO);
    glGenBuffers(1, &graphVBO);

    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

    std::vector<Coords> coords = convertToCartesian(calcInterval(0, 10, lineConstant, step));
    float maxCoord = findBiggest(coords);
    std::vector<GLfloat> vertices = normalize(coords, maxCoord);
    const GLuint vertCount = vertices.size() / 2;

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    vertices.clear();
    coords.clear();
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(graphVAO);
    glBindBuffer(GL_ARRAY_BUFFER, graphVBO);

    std::vector<Coords> graph = getGraphVertices(TICK_COUNT);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Coords) * graph.size(), graph.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Shaders

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    TextRenderer textRenderer("0123456789.XY", SCR_WIDTH, SCR_HEIGHT);

    Shader mainShader("assets/shaders/main.vert.glsl", "assets/shaders/main.frag.glsl");
    Shader textShader("assets/shaders/text.vert.glsl", "assets/shaders/text.frag.glsl");

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(mainShader.Get());
        glUniform1f(glGetUniformLocation(mainShader.Get(), "scale"), scale);
        glUniform2f(glGetUniformLocation(mainShader.Get(), "offset"), offset.x - scrOffset.x, offset.y - scrOffset.y);
        

        glUniform3f(glGetUniformLocation(mainShader.Get(), "inColor"), 0.0f, 1.0f, 1.0f);
        glBindVertexArray(lineVAO);
        glDrawArrays(GL_LINE_STRIP, 0, vertCount);

        glUniform3f(glGetUniformLocation(mainShader.Get(), "inColor"), 1.0f, 1.0f, 1.0f);
        glBindVertexArray(graphVAO);
        glDrawArrays(GL_LINES, 0, graph.size());

        glUseProgram(textShader.Get());
        glUniform1f(glGetUniformLocation(textShader.Get(), "scale"), scale);
        glUniform2f(glGetUniformLocation(textShader.Get(), "offset"), offset.x - scrOffset.x, offset.y - scrOffset.y);

        for (size_t i = 0; i < TICK_COUNT; ++i) {
            float tickValue = -maxCoord + 2 * maxCoord / (TICK_COUNT - 1) * i;
            float textPos   = -1.0 + 2.0 / (TICK_COUNT - 1) * i;

            textRenderer.renderFloat(tickValue, textPos, -0.1, scale * 0.5, { offset.x + scrOffset.x, offset.y + scrOffset.y });
            textRenderer.renderFloat(tickValue, -0.1, textPos + 0.02, scale * 0.5, { offset.x + scrOffset.x, offset.y + scrOffset.y });
        }
        textRenderer.render("X", 1.0, 0.05, scale * 0.5, { offset.x + scrOffset.x, offset.y + scrOffset.y });
        textRenderer.render("Y", 0.05, 1.0, scale * 0.5, { offset.x + scrOffset.x, offset.y + scrOffset.y });

        manageInput();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lineVAO);
    glDeleteVertexArrays(1, &graphVAO);
    glDeleteBuffers(1, &lineVBO);
    glDeleteBuffers(1, &graphVBO);
    
    glfwTerminate();
}