#pragma once

#include <entityx/entityx.h>

using namespace entityx;

class CScript 
{
public:
    CScript(Entity* entity);

    virtual void Start() = 0;
    virtual void Update(TimeDelta dt) = 0;

    // Collision detection
    virtual void BeginContact(Entity* entityA, Entity* entityB);
    virtual void EndContact(Entity* entityA, Entity* entityB);

protected:
    Entity entity;
};
