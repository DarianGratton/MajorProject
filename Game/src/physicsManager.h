#pragma once

#include <box2d/box2d.h>

#include "Renderer/b2Renderer.h"
#include "logger.h"

class PhysicsManager {
public:
    static PhysicsManager& instance() {
        static PhysicsManager *instance = new PhysicsManager();
        return *instance;
    }

    PhysicsManager(PhysicsManager const&) = delete;
    void operator=(PhysicsManager const&) = delete;

    void update();
    void draw(glm::mat4 proj, glm::mat4 view);

    void createWorld(float gravX, float gravY);
    b2World* getWorld();

private:
    PhysicsManager() {}

    b2World* world;
};
