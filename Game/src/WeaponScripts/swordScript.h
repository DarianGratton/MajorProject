#pragma once

#include <entityx/entityx.h>

#include "weaponScript.h"

class SwordScript : public WeaponScript {
public:
    SwordScript(Entity* entity, float spriteHeight, float spriteWidth);

    void start() override;
    void update() override;
    void useWeapon() override;

    // Collision detection
    void beginContact() override;
    void endContact() override;

private:
    Entity player;
    float spriteOffset;
    unsigned int cooldown;

    int getPlayerDirection();
};