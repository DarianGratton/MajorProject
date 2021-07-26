#pragma once

#include <entityx/entityx.h>

#include "weaponScript.h"

class ShieldScript : public WeaponScript {
public:
    ShieldScript(entityx::Entity* entity);

    void start() override;
    void update(TimeDelta dt) override;
    void useWeapon() override;

    // Collision detection
    void beginContact(Entity* entityA, Entity* entityB) override;
    void endContact(Entity* entityA, Entity* entityB) override;

private:
    Entity player;
    unsigned int cooldown;
};