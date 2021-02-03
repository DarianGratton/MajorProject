#include <iostream>

#include <glfw/glfw3.h>
#include <glad/gald.h>

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

}

int main(int argc, char** argv) {
    Logger::init();
    LOG_INFO("Logger Initialized");

    if (!initGLFW()) {
        LOG_ERROR("Error occurred in initializing GLFW");
    }
    LOG_INFO("GLFW Initialized");

    return 0;
}