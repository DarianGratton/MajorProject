#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "ECS.h"
#include "logger.h"
#include "input.h"

// GLFW Variables
GLFWwindow* window;
const char* GAME_NAME = "Game";

// TODO: Change to a reponsive value
// Window Sizes:
// 1240 x 960 (Large)
// 1080 x 720 (Medium)
// 960 x 540  (Small)
const unsigned int WINDOW_WIDTH = 1080;
const unsigned int WINDOW_HEIGHT = 720;

// Time
std::chrono::high_resolution_clock hrclock;
std::chrono::milliseconds lastTime;
std::chrono::milliseconds deltaTime;

bool initGLFW() {
    // Initialize GLFW
    if (!glfwInit()) {
        LOG_ERROR("GLFW could not be initialized");
        return 0;
    }

    // Setup window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME, NULL, NULL);
    if (!window) {
        LOG_ERROR("GLFW Window failed to be created");
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    // Setup input
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

    return 1;
}

bool initOpenGL() {
    // Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("GLAD could not be initialized");
        return 0;
    }

    // Setup layers
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 1;
}

TimeDelta calculateDT() {
    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(hrclock.now().time_since_epoch()) - lastTime;
    lastTime += deltaTime;
    TimeDelta dt = deltaTime.count();
    dt *= 0.001f; //convert to seconds
    return dt;
}

int main(int argc, char** argv) {
    // Initialization
    Logger::init();
    LOG_INFO("Logger Initialized");

    if (!initGLFW()) {
        LOG_ERROR("Error occurred in initializing GLFW");
        return -1;
    }
    LOG_INFO("GLFW Initialized");

    if (!initOpenGL) {
        LOG_ERROR("Error occurred in initializing GLAD");
        return -1;
    }
    LOG_INFO("GLAD Initialized");

    Input::instance().init(window);
    LOG_INFO("Input Initialized");

    ECS::instance().init();
    LOG_INFO("ECS Initialized");

    // To calculate time between frames
    lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(hrclock.now().time_since_epoch());
    LOG_INFO("DeltaTime Initialized");

    // Game Loop
    while (!glfwWindowShouldClose(window)) {
        // Clear any input from last frame
        Input::instance().clear();

        // Poll for and process events
        glfwPollEvents();

        // Update Systems
        TimeDelta dt = calculateDT();
        ECS::instance().update(dt);

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    // Deallocate
    glfwTerminate();

    LOG_INFO("Program ended successfully");
    return 0;
}
