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
    inline bool IsGamePaused() { return isGamePaused; }
    inline bool IsGameEnded() { return isGameEnded; }
    
    // Agent Functions
    inline void SetAutomaticTraining(bool training) { isAutomaticallyTraining = training; }
    inline void SetManualTraining(bool training) { isManuallyTraining = training; }
    inline void SetTerminalState(bool terminal) { isTerminalState = terminal; }
    inline bool IsAutomaticallyTraining() { return isAutomaticallyTraining; }
    inline bool IsManuallyTraining() { return isManuallyTraining; }
    inline bool IsAgentTraining() { return isAutomaticallyTraining || isManuallyTraining; }
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
    // NOTE: Probably should be part of a Singleton class for modularity.
    //       However it should be fine unless it gets overly complex.
    bool isAutomaticallyTraining = false;
    bool isManuallyTraining = false;
    bool isTerminalState = false;
};
