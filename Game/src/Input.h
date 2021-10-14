#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <vector>

class Input 
{
public:
    static Input& Instance()
    {
        static Input *instance = new Input();
        return *instance;
    }

    void Init(GLFWwindow* window);

    void PressKey(int key);
    void ReleaseKey(int key);
    void PressMouseButton(int button);

    bool IsKeyPressed(int key);
    bool IsMousePressed(bool isLeft);

    void CursorPositionCallback(float xpos, float ypos);
    void CursorEnterCallback(int entered);

    float GetCursorPosition(bool isX);

    void Clear();

private:
    Input() {}
    Input(Input const&) = delete;
    void operator=(Input const&) = delete;

    std::vector<int> pressedKeys;
    bool leftMousePressed = false;
    bool rightMousePressed = true;
    float mousePosX = 0;
    float mousePosY = 0;
};