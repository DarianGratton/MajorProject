#pragma once

#include <box2d/box2d.h>
#include <entityx/entityx.h>

#include "../components.h"
#include "../events.h"

#include "../logger.h"

using namespace entityx;

class PhysicsSystem : public System<PhysicsSystem>, public Receiver<PhysicsSystem> {
public:
    void configure(EventManager& events) override {
        events.subscribe<SceneLoad>(*this);
    }

    void update(EntityManager& es, EventManager& events, TimeDelta dt) override {
        
        if (world = nullptr)
            return;

        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        world->Step(dt, velocityIterations, positionIterations);

        es.each<RigidBody>([dt](Entity entity, RigidBody &rigidBody) {
            b2Vec2 position = rigidBody.body->GetPosition();
            float angle = rigidBody.body->GetAngle();
            // printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
        });

    }

    void receive(const SceneLoad& sl) {
        
        // Setup physics world when scene loads
        b2Vec2 gravity(-10.0f, 0.0f);
        world = new b2World(gravity);

        // Set up bodies and fixture in physics world
        ComponentHandle<RigidBody> physicsComp;
        for (Entity e : sl.entities->entities_with_components(physicsComp)) {
            physicsComp = e.component<RigidBody>();
            physicsComp.get()->body = world->CreateBody(&physicsComp.get()->bodyDef);
            physicsComp.get()->createFixture();
        }
    }

private:
    b2World* world;    
};
