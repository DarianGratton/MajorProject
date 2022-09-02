#pragma once

#include <glad/glad.h> 
#include <glfw/glfw3.h>
#include <entityx/entityx.h>

#include <chrono>

using namespace entityx;

class Game 
{
public:
    // Returns the class instance
    static Game& Instance() 
    {
        static Game *instance = new Game();
        return *instance;
    }

    // Remove the copy and = operators
    Game(Game const&) = delete;
    void operator=(Game const&) = delete;

    bool Init();
    void Run();
    void CleanUp();

    inline void EndGame() { isGameEnded = true; } 
    inline void PauseGame() { isGamePaused = true; }
    inline void UnpauseGame() { isGamePaused = false; } 
    inline void SetTerminalState(bool terminal) { isTerminalState = terminal; }
    inline bool IsGamePaused() { return isGamePaused; }
    inline bool IsGameEnded() { return isGameEnded; }
    inline bool IsAgentTraining() { return isAgentTraining; }
    inline bool IsTerminalState() { return isTerminalState; }

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
    bool isGamePaused = false;
    bool isGameEnded = false;

    // Agent
    bool isAgentTraining = false;
    bool isTerminalState = false;
};
