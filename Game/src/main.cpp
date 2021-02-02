#include <iostream>

#include <glfw/glfw3.h>

#include "Logger.h"

int main(int argc, char** argv) {
    Logger::init();
    LOG_INFO("Logger Initialized");

    if (!glfwInit()) {
        LOG_ERROR("GLFW could not be initialized");
    }

    return 0;
}