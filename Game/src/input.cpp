#include "input.h"

#include "logger.h"

void Input::init(GLFWwindow* window) {
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS) {
                Input::instance().pressKey(key);
            } else if (action == GLFW_RELEASE) {
                Input::instance().releaseKey(key);
            }
        });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
            Input::instance().cursorPositionCallback(xpos, ypos);
        });

    glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) {
            Input::instance().cursorEnterCallback(entered);
        });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
            Input::instance().pressMouseButton(button);
        });
}

void Input::pressKey(int key) {
    pressedKeys.push_back(key);
}

void Input::releaseKey(int key) {
    for (auto i = pressedKeys.begin(); i != pressedKeys.end(); ++i) {
        if (*i == key) {
            pressedKeys.erase(i);
            return;
        }
    }
}

void Input::pressMouseButton(int button) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightMousePressed = 1;    
    } else if (button == GLFW_MOUSE_BUTTON_LEFT) {
        leftMousePressed = 1;
    } 
}

bool Input::isKeyPressed(int key) {
    for (int i = 0; i < pressedKeys.size(); ++i) {
        if (pressedKeys.at(i) == key) {
            return 1;
        }
    }
    return 0;
}

bool Input::isMousePressed(bool isLeft) {
    if (isLeft) {
        return leftMousePressed;
    } else {
        return rightMousePressed;
    }
}

void Input::cursorPositionCallback(float xpos, float ypos) {
    mousePosX = xpos;
    mousePosY = ypos;
}

void Input::cursorEnterCallback(int entered) {
    // Entered the window
    // LOG_INFO("Window Entered");
}

float Input::getCursorPosition(bool isX) {
    if (isX) {
        return mousePosX;
    } else if (isX) {
        return mousePosY;
    }
    return -1;
}

void Input::clear() {
    leftMousePressed = false;
    rightMousePressed = false;
}
