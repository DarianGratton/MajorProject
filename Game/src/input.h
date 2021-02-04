#pragma once

#include <vector>

class Input {
public:

    static Input& instance() {
        static Input *instance = new Input();
        return *instance;
    }

    void pressKey(int key);
    void releaseKey(int key);
    void pressMouseButton(int button);

    bool isKeyPressed(int key);
    bool isMousePressed(bool isLeft);

    void cursorPositionCallback(float xpos, float ypos);
    float getCursorPosition(bool isX);

    void clear();

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