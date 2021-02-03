#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Logger.h"

// GLFW Variables
GLFWwindow* window;
const char * GAME_NAME = "Game";
// TODO: Change to a robust value (not hardcoded)
const unsigned int WINDOW_WIDTH = 960;
const unsigned int WINDOW_HEIGHT = 540;

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

    // TODO: Setup input

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

    // Game Loop
    while (!glfwWindowShouldClose(window)) {
        // Poll for and process events
        glfwPollEvents();

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    // Deinitialization
    glfwTerminate();

    LOG_INFO("Program ended");
    return 0;
}