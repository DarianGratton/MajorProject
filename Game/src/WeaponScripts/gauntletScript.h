#pragma once

#include <entityx/entityx.h>

#include "weaponScript.h"

class GauntletScript : public WeaponScript {
public:
    GauntletScript(Entity* entity, float spriteHeight, float spriteWidth);

    void start() override;
    void update(TimeDelta dt) override;
    void useWeapon() override;

    // Collision detection
    void beginContact(Entity* entityA, Entity* entityB) override;
    void endContact(Entity* entityA, Entity* entityB) override;

private:
    Entity player;
    float spriteOffset;
    float attackRate;
    float spriteHeight;
    float spriteWidth;

    int getPlayerDirection();
};