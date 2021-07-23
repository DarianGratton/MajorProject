#pragma once

#include <entityx/entityx.h>

#include "../Physics/physicsManager.h"
#include "../components.h"
#include "../events.h"
#include "../Renderer/b2Renderer.h"

#include "../logger.h"

using namespace entityx;

class PhysicsSystem : public System<PhysicsSystem>, public Receiver<PhysicsSystem> {
public:
    void configure(EventManager& events) override {
        events.subscribe<SceneLoad>(*this);
    }

    void update(EntityManager& es, EventManager& events, TimeDelta dt) override {
        
        // Update and run the physics 
        PhysicsManager::instance().update();

        // es.each<RigidBody>([dt](Entity entity, RigidBody &rigidBody) {
        //     b2Vec2 position = rigidBody.body->GetPosition();
        //     float angle = rigidBody.body->GetAngle();
        //     printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
        // });
    }

    void receive(const SceneLoad& sl) {

        // Setup physics world when scene loads
        PhysicsManager::instance().createWorld(0.0f, 0.0f);

        // Set up bodies and fixture in physics world
        ComponentHandle<RigidBody> physicsComp;
        for (Entity e : sl.entities->entities_with_components(physicsComp)) {
            physicsComp = e.component<RigidBody>();
            physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
            physicsComp.get()->createFixture();
        }
    }
};
