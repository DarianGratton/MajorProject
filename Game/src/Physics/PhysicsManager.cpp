
#include "PhysicsManager.h"

void PhysicsManager::Update() 
{
    if (world == nullptr) 
    {
        LOG_WARN("PhysicsManager: World has yet to be created");
        return;
    }

    float timeStep = 1.0f / 120.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    world->Step(timeStep, velocityIterations, positionIterations);  
}

void PhysicsManager::Draw(glm::mat4 proj, glm::mat4 view) 
{
    if (world == nullptr) 
    {
        LOG_WARN("PhysicsManager: World has yet to be created");
        return;
    }

    Box2DRenderer box2DRenderer;
    box2DRenderer.proj = proj;
    box2DRenderer.view = view;

    world->SetDebugDraw(&box2DRenderer);
    box2DRenderer.SetFlags(b2Draw::e_shapeBit);
    world->DebugDraw();
}

void PhysicsManager::CreateWorld(float gravX, float gravY) 
{
    b2Vec2 gravity(gravX, gravY);
    world = new b2World(gravity);

    contactListener = new ContactListener();
    world->SetContactListener(contactListener);
}

PhysicsManager::bodyCategory PhysicsManager::IntToCategory(int i) 
{
    // TODO: Might mess up physics so leaving unchanged for now
    // return static_cast<PhysicsManager::bodyCategory>(i);
    switch(i) 
    {
        case 1:
            return BOUNDARY;
        case 2:
            return PLAYER;
        case 3:
            return ENEMY;
        case 4: 
            return PLAYERWEAPON;
        case 5:
            return ENEMYWEAPON;
        default:
            return BOUNDARY;
    }
}

b2World* PhysicsManager::GetWorld() 
{
    return world;
}
