#include "Input.h"

#include "Logger.h"

void Input::Init(GLFWwindow* window) 
{
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                Input::Instance().PressKey(key);
            } 
            else if (action == GLFW_RELEASE) 
            {
                Input::Instance().ReleaseKey(key);
            }
        });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) 
        {
            Input::Instance().CursorPositionCallback(xpos, ypos);
        });

    glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) 
        {
            Input::Instance().CursorEnterCallback(entered);
        });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) 
        {
            Input::Instance().PressMouseButton(button);
        });
}

void Input::PressKey(int key) 
{
    pressedKeys.push_back(key);
}

void Input::ReleaseKey(int key) 
{
    for (auto i = pressedKeys.begin(); i != pressedKeys.end(); ++i) 
    {
        if (*i == key) 
        {
            pressedKeys.erase(i);
            return;
        }
    }
}

void Input::PressMouseButton(int button) 
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT) 
    {
        rightMousePressed = 1;    
    } 
    else if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        leftMousePressed = 1;
    } 
}

bool Input::IsKeyPressed(int key)
{
    for (int i = 0; i < pressedKeys.size(); ++i) 
    {
        if (pressedKeys.at(i) == key) 
            return 1;
    }
    return 0;
}

bool Input::IsMousePressed(bool isLeft) 
{
    if (isLeft) 
    {
        return leftMousePressed;
    } 
    else 
    {
        return rightMousePressed;
    }
}

void Input::CursorPositionCallback(float xpos, float ypos) 
{
    mousePosX = xpos;
    mousePosY = ypos;
}

void Input::CursorEnterCallback(int entered) 
{
    // Entered the window
    // LOG_INFO("Window Entered");
}

float Input::GetCursorPosition(bool isX) 
{
    if (isX) 
    {
        return mousePosX;
    } 
    else if (isX) 
    {
        return mousePosY;
    }
    return -1;
}

void Input::Clear() {
    leftMousePressed = false;
    rightMousePressed = false;
}
