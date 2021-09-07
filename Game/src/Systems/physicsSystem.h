#pragma once

#include <entityx/entityx.h>

#include "../Physics/PhysicsManager.h"
#include "../Renderer/B2Renderer.h"
#include "../Components.h"
#include "../Events.h"
#include "../Logger.h"

using namespace entityx;

class PhysicsSystem : public System<PhysicsSystem>, public Receiver<PhysicsSystem> 
{
public:
    void configure(EventManager& events) override 
    {
        events.subscribe<SceneLoad>(*this);
    }

    void update(EntityManager& es, EventManager& events, TimeDelta dt) override 
    {
        // Update and run the physics 
        PhysicsManager::Instance().Update();
    }

    void receive(const SceneLoad& sl) 
    {
        // Setup physics world when scene loads
        PhysicsManager::Instance().CreateWorld(0.0f, 0.0f);

        // Set up bodies and fixture in physics world
        ComponentHandle<RigidBody> physicsComp;
        for (Entity e : sl.entities->entities_with_components(physicsComp)) 
        {
            physicsComp = e.component<RigidBody>();
            physicsComp.get()->body = PhysicsManager::Instance().GetWorld()->CreateBody(&physicsComp.get()->bodyDef);
            physicsComp.get()->CreateFixture();
            physicsComp.get()->SetUserData(&e);
        }
    }
};
