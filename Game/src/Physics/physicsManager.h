#pragma once

#include <box2d/box2d.h>

#include "ContactListener.h"
#include "../Renderer/B2Renderer.h"
#include "../Logger.h"

class PhysicsManager 
{
public:
    enum bodyCategory 
    {
        BOUNDARY     = 0x0001,
        PLAYER       = 0x0002,
        ENEMY        = 0x0004,
        PLAYERWEAPON = 0x0008,
        ENEMYWEAPON  = 0x00010,
    };

    static PhysicsManager& Instance() 
    {
        static PhysicsManager *instance = new PhysicsManager();
        return *instance;
    }

    PhysicsManager(PhysicsManager const&) = delete;
    void operator=(PhysicsManager const&) = delete;

    void Update();
    void Draw(glm::mat4 proj, glm::mat4 view);

    void CreateWorld(float gravX, float gravY);
    bodyCategory IntToCategory(int i);

    b2World* GetWorld();

private:
    PhysicsManager() {}

    b2World* world;
    ContactListener* contactListener;
};
