#pragma once

#include <entityx/entityx.h>

class CScript {
public:
    CScript(entityx::Entity* entity);

    virtual void start() = 0;
    virtual void update() = 0;

private:
    entityx::Entity* entity;
};
