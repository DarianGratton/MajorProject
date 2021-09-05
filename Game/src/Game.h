#pragma once

#include <glad/glad.h> 
#include <glfw/glfw3.h>
#include <entityx/entityx.h>

#include <chrono>

using namespace std;
using namespace entityx;

class Game {
public:
    // Returns the class instance
    static Game& Instance() {
        static Game *instance = new Game();
        return *instance;
    }

    // Remove the copy and = operators
    Game(Game const&) = delete;
    void operator=(Game const&) = delete;

    bool Init();
    void Run();
    void CleanUp();

private:
    // Private default constructor
    Game() {}

    // Private functions
    bool InitGLFW();
    TimeDelta CalculateDT();

    // GLFW
    GLFWwindow* window;
    const char* GAME_NAME = "Game";
    const unsigned int WINDOW_WIDTH = 1080; // Note: Magic Numbers
    const unsigned int WINDOW_HEIGHT = 720;

    // Time
    std::chrono::high_resolution_clock hrclock;
    std::chrono::milliseconds lastTime;
    std::chrono::milliseconds deltaTime;

    // Game
    bool isGamePaused;
    bool isGameEnded;
};
