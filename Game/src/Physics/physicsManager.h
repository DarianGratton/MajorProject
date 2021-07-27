#pragma once

#include <box2d/box2d.h>

#include "../Renderer/b2Renderer.h"
#include "contactListener.h"
#include "../logger.h"

class PhysicsManager {
public:
    enum bodyCategory {
        BOUNDARY     = 0x0001,
        PLAYER       = 0x0002,
        ENEMY        = 0x0004,
        PLAYERWEAPON = 0x0008,
        ENEMYWEAPON  = 0x00010,
    };

    static PhysicsManager& instance() {
        static PhysicsManager *instance = new PhysicsManager();
        return *instance;
    }

    PhysicsManager(PhysicsManager const&) = delete;
    void operator=(PhysicsManager const&) = delete;

    void update();
    void draw(glm::mat4 proj, glm::mat4 view);

    void createWorld(float gravX, float gravY);
    bodyCategory intToCategory(int i);

    b2World* getWorld();

private:
    PhysicsManager() {}

    b2World* world;
    ContactListener* contactListener;
};
