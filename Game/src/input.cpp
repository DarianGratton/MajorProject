#include "input.h"

#include <glfw/glfw3.h>

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

float Input::getCursorPosition(bool isX) {
    if (isX) {
        return mousePosX;
    } else if (isX) {
        return mousePosY;
    }
}

void Input::clear() {
    leftMousePressed = false;
    rightMousePressed = false;
}
