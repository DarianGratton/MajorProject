#pragma once

#include <entityx/entityx.h>
#include <box2d/box2d.h>

#include "../ECS.h"
#include "../Logger.h"

using namespace entityx;

struct RigidBody 
{
    RigidBody(float xpos, float ypos, float hx, float hy, 
        float density = 0.0f, float friction = 1.0f, bool isDynamicBody = false, 
        uint16 categoryBits = 0, uint16 maskBits = 0, bool setSensor = true)
        : density(density), friction(friction), isDynamicBody(isDynamicBody), categoryBits(categoryBits), maskBits(maskBits) 
    {            
        // Create rigid body
        if (isDynamicBody)
            bodyDef.type = b2_dynamicBody;

        bodyDef.fixedRotation = true;
        bodyDef.position.Set(xpos, ypos);

        // Attach a polygon shape
        collisionBox.SetAsBox(hx, hy);
    }

    void CreateFixture() 
    {
        if (body == nullptr) 
        {
            LOG_ERROR("RigidBody createFixture: Body has not yet been created in the physics world");
            return;
        }

        if (categoryBits != 0) 
            fixtureDef.filter.categoryBits = categoryBits;

        if (maskBits != 0)
            fixtureDef.filter.maskBits = maskBits;

        if (isDynamicBody) 
        {
            fixtureDef.shape = &collisionBox;
            fixtureDef.density = density;
            fixtureDef.friction = friction; 
            body->CreateFixture(&fixtureDef);
        } 
        else 
        {
            body->CreateFixture(&collisionBox, density);
        }
    }

    void SetUserData(Entity* e) 
    {
        if (body == nullptr) 
        {
            LOG_ERROR("RigidBody createFixture: Body has not yet been created in the physics world");
            return;
        }

        entity = ECS::Instance().entities.get(e->id());
        body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entity);
    }

    void SetSensor(bool sensor)
    {
        body->GetFixtureList()->SetSensor(sensor);
    }

    float density;
    float friction;
    bool isDynamicBody;
    uint16 categoryBits;
    uint16 maskBits;
    b2BodyDef bodyDef;
    b2PolygonShape collisionBox;
    b2FixtureDef fixtureDef;
    b2Body* body;

private:
    Entity entity;
};