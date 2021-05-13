#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

namespace Input {
    // Don't care anymore
    // fite me

    bool upPressed;
    bool leftPressed;
    bool downPressed;
    bool rightPressed;
    bool zoomInPressed;
    bool zoomOutPressed;

    void keyboardCallback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode) {
        switch (key) {
            case GLFW_KEY_ESCAPE: {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            }
            case GLFW_KEY_W:
            case GLFW_KEY_UP: {
                upPressed = (action != GLFW_RELEASE);
                break;
            }
            case GLFW_KEY_A:
            case GLFW_KEY_LEFT: {
                leftPressed = (action != GLFW_RELEASE);
                break;
            }
            case GLFW_KEY_S:
            case GLFW_KEY_DOWN: {
                downPressed = (action != GLFW_RELEASE);
                break;
            }
            case GLFW_KEY_D:
            case GLFW_KEY_RIGHT: {
                rightPressed = (action != GLFW_RELEASE);
                break;
            }
            case GLFW_KEY_Q: {
                zoomInPressed = (action != GLFW_RELEASE);
                break;
            }
            case GLFW_KEY_E: {
                zoomOutPressed = (action != GLFW_RELEASE);
                break;
            }
        }
    }
}

#endif