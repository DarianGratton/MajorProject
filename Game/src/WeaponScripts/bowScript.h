#pragma once

#include <entityx/entityx.h>
#include <vector>

#include "weaponScript.h"

class BowScript : public WeaponScript {
public:
    BowScript(Entity* entity, float spriteHeight, float spriteWidth);

    void start() override;
    void update(TimeDelta dt) override;
    void useWeapon() override;

    // Collision detection
    void beginContact() override;
    void endContact() override;

private:
    Entity player;
    float spriteOffset;
    float spriteHeight;
    float spriteWidth;
    float projectileSpeed;
    float projectileLifespan;
    float cooldownBetweenShots;

    // Entity and it's direction
    std::vector<std::pair<Entity, int>> projectiles;
    std::vector<float> projectilesTimeElapsed;

    int getPlayerDirection();
    void spawnArrow();
};