#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <box2d/box2d.h>

#include "ECS.h"
#include "logger.h"
#include "input.h"
#include "sceneManager.h"

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
    entityx::Entity e2 = ECS::instance().entities.create();
    e2.assign<SpriteVertices>();
    e2.assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    e2.assign<TextureComp>("src/Assets/textures/platformChar_idle.png");
    e2.assign<Transform>(0.0f, 0.0f, 0.0f, 0, 0, 0, 1, 2);
    e2.assign<Script>("PlayerScript", &e2);
    e2.assign<Camera>((float)WINDOW_WIDTH / 2 * -1, (float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT / 2 * -1, (float)WINDOW_HEIGHT / 2, -1.0f, 1.0f);
    
    entityx::Entity textEntity = ECS::instance().entities.create();
    textEntity.assign<TextSprite>("Hello world 2");
    // textEntity.assign<Audio>("src/Assets/audio/RDR2.mp3");
    textEntity.assign<Transform>(-25.0f, 1.0f, 0.0f, 0, 0, 0, 1, 2);
}

void sceneTest() {
    SceneManager::instance().addScene("Test", "src/Assets/scenes/arena.xml");
    SceneManager::instance().loadScene("Test");
}

TimeDelta calculateDT() {
    deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(hrclock.now().time_since_epoch()) - lastTime;
    lastTime += deltaTime;
    TimeDelta dt = deltaTime.count();
    dt *= 0.001f; //convert to seconds
    return dt;
}

void PhysicsTest() {
    // Define gravity vector
    b2Vec2 gravity(-10.0f, 0.0f);

    // Create world object
    // Note: Creating on the stack so the world must remain in scope
    b2World world(gravity);

    // Bodies are built using the following steps
    // 1. Define a body with position, damping, etc.
    // 2. Use the world object to create the body
    // 3. Define fixtures with a shape, friction, density, etc. 
    // 4. Create fixtures on the body

    // Step 1: Create Ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(-10.0f, 0.0f);

    // Step 2: Body is passed to the world object to create the ground body.
    // Bodies are static by default and they don't collide with other static bodys
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Step 3: We create a ground polygon
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    // Step 4: Create shape fixture
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Creating a dynamic body
    // Create the body using createbody
    // Must set the body type to b2_dynamicBody if you want the body to move in response forces
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    // Create and attach a polygon shape using a fixture definition
    // Create box shape
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Create fixture definition using the box
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    // Create fixture
    body->CreateFixture(&fixtureDef);

    // Simulating the world
    float timeStep = 1.0f / 60.0f;

    // Iteration count
    // More equals better accuracy but poorer performance
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int32 i = 0; i < 60; ++i)
    {
        world.Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }

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

    if (!BASS_Init(-1, 44100, 0, NULL, NULL)) {
        LOG_ERROR("Bass could not be initialized");
        return -1;
    }
    LOG_INFO("Bass Initialized");

    ECS::instance().init();
    LOG_INFO("ECS Initialized");

    // To calculate time between frames
    lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(hrclock.now().time_since_epoch());
    LOG_INFO("DeltaTime Initialized");

    // renderingTest();
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
