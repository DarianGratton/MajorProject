#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "ECS.h"
#include "logger.h"
#include "input.h"
#include "sceneManager.h"

// FreeType
#include "ft2build.h"
#include FT_FREETYPE_H

// TODO: Fix bug with program sometimes not ending after window closes

// GLFW Variables
static GLFWwindow* window;
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
} 

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
    glfwSwapInterval(1); //VSYNC
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

void renderingTest() {
    for (int i = 0; i < 100; i++) {
    entityx::Entity e2 = ECS::instance().entities.create();
    //e2.assign<AudioSource>(new Sound("kick-trimmed.wav", true));
    e2.assign<SpriteVertices>();
    e2.assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    e2.assign<TextureComp>("src/Assets/textures/platformChar_idle.png");
    e2.assign<Transform>(0.0f, 0.0f, 0.0f, 0, 0, 0, 1, 2);
    e2.assign<Camera>((float)WINDOW_WIDTH / 2 * -1, (float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2 * -1, (float)WINDOW_HEIGHT / 2, -1.0f, 1.0f);
    }
}

void sceneTest() {
    SceneManager::instance().addScene("Test", "src/Assets/scenes/test.xml");
    SceneManager::instance().loadScene("Test");
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

    // TODO: Find out how to isolate in different function (currently breaks)
    // Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("GLAD could not be initialized");
        return -1;
    }

    // Setup layers
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    LOG_INFO("GLAD Initialized");

    Input::instance().init(window);
    LOG_INFO("Input Initialized");

    ECS::instance().init();
    LOG_INFO("ECS Initialized");

    // Initialize FreeType
    FT_Library library;
    if (FT_Init_FreeType(&library)) {
        LOG_ERROR("FreeType could not be initialized");
        return -1;
    }
    LOG_INFO("FreeType Initialized");

    // To calculate time between frames
    lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(hrclock.now().time_since_epoch());
    LOG_INFO("DeltaTime Initialized");

    renderingTest();
    sceneTest();

    // Game Loop
    LOG_INFO("Program started successfully");
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
