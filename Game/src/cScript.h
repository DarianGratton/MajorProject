#pragma once

#include <entityx/entityx.h>

using namespace entityx;

class CScript {
public:
    CScript(Entity* entity);

    virtual void start() = 0;
    virtual void update() = 0;

    // Collision detection
    virtual void beginContact();
    virtual void endContact();

protected:
    Entity entity;
};
