#pragma once

#include <entityx/entityx.h>
#include <box2d/box2d.h>

#include "../ECS.h"
#include "../logger.h"

using namespace entityx;

struct RigidBody {
    RigidBody(float xpos, float ypos, float hx, float hy, float density = 0.0f, float friction = 1.0f, bool isDynamicBody = false) 
        : density(density), friction(friction), isDynamicBody(isDynamicBody) {
                
        // Create rigid body
        if (isDynamicBody)
            bodyDef.type = b2_dynamicBody;

        bodyDef.fixedRotation = true;
        bodyDef.position.Set(xpos, ypos);

        // Attach a polygon shape
        collisionBox.SetAsBox(hx, hy); 

    }

    void createFixture() {
        if (body == nullptr) {
            LOG_ERROR("RigidBody createFixture: Body has not yet been created in the physics world");
            return;
        }

        if (isDynamicBody) {
            fixtureDef.shape = &collisionBox;
            fixtureDef.density = density;
            fixtureDef.friction = friction; 
            body->CreateFixture(&fixtureDef);
        } else {
            body->CreateFixture(&collisionBox, density);
        }
    }

    void setUserData(Entity* e) {
        if (body == nullptr) {
            LOG_ERROR("RigidBody createFixture: Body has not yet been created in the physics world");
            return;
        }

        entity = ECS::instance().entities.get(e->id());
        body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entity);
    }

    float density;
    float friction;
    bool isDynamicBody;
    b2BodyDef bodyDef;
    b2PolygonShape collisionBox;
    b2FixtureDef fixtureDef;
    b2Body* body;

private:
    Entity entity;
};