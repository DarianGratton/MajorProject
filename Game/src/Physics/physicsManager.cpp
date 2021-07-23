
#include "physicsManager.h"

void PhysicsManager::update() {
    if (world == nullptr) {
        LOG_WARN("PhysicsManager: World has yet to be created");
        return;
    }

    float timeStep = 1.0f / 120.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    world->Step(timeStep, velocityIterations, positionIterations);  
}

void PhysicsManager::draw(glm::mat4 proj, glm::mat4 view) {
    if (world == nullptr) {
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

void PhysicsManager::createWorld(float gravX, float gravY) {
    b2Vec2 gravity(gravX, gravY);
    world = new b2World(gravity);

    contactListener = new ContactListener();
    world->SetContactListener(contactListener);
}

b2World* PhysicsManager::getWorld() {
    return world;
}
