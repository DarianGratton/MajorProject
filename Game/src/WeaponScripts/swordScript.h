#pragma once

#include <entityx/entityx.h>

#include "weaponScript.h"

class SwordScript : public WeaponScript {
public:
    SwordScript(Entity* entity, float spriteHeight, float spriteWidth);

    void start() override;
    void update(TimeDelta dt) override;
    void useWeapon() override;

    // Collision detection
    void beginContact(Entity* entityA, Entity* entityB) override;
    void endContact(Entity* entityA, Entity* entityB) override;

private:
    Entity player;
    float spriteOffset;
    unsigned int cooldown;

    int getPlayerDirection();
};