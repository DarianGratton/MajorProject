
#include "Game.h"

#include "logger.h"
#include "ECS.h"
#include "input.h"
#include "sceneManager.h"

// GLFW: Whenever the window size changed (by OS or user resize) this callback function executes
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool Game::Init() 
{
    // Logger
    Logger::Init();

    // GLFW
    if (!InitGLFW()) {
        LOG_ERROR("Error occurred in initializing GLFW");
        return -1;
    }

    // GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("GLAD could not be initialized");
        return -1;
    }

    // Input (GLFW)
    Input::instance().init(window);

    // Audio (BASS)
    if (!BASS_Init(-1, 44100, 0, NULL, NULL)) {
        LOG_ERROR("Bass could not be initialized");
        return -1;
    }

    // Entity Component System (EntityX)
    ECS::instance().init();

    // Setup layers
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // To calculate time between frames
    lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(hrclock.now().time_since_epoch());

    return 1;
}

bool Game::InitGLFW()
{
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
    glfwSwapInterval(1); // VSYNC
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Setup input
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

    return 1;
}

TimeDelta Game::CalculateDT()
{
    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(hrclock.now().time_since_epoch()) - lastTime;
    lastTime += deltaTime;
    TimeDelta dt = deltaTime.count();
    dt *= 0.001f; //convert to seconds
    return dt;
}

void Game::Run()
{
    // Game Loop
    while (!glfwWindowShouldClose(window) && !ECS::instance().gameEnded) {
        // Clear any input from last frame
        Input::instance().clear();

        // Poll for and process events
        glfwPollEvents();

        // Update Systems
        TimeDelta dt = CalculateDT();
        ECS::instance().update(dt);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        // float framerate = 1 / dt;
        // LOG_INFO(framerate);
    }
}

void Game::CleanUp()
{
    glfwTerminate();
}
