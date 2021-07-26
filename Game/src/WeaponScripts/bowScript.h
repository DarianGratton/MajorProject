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
    void beginContact(Entity* entityA, Entity* entityB) override;
    void endContact(Entity* entityA, Entity* entityB) override;

private:
    Entity player;
    float spriteOffset;
    float spriteHeight;
    float spriteWidth;
    float projectileSpeed;
    float projectileLifespan;
    float fireRate;
    int arrowNumber;

    // Projectile data structures
    std::vector<std::pair<Entity, int>> projectiles;
    std::vector<float> projectilesTimeElapsed;
    std::vector<std::string> projectilesFlaggedForDeletion; 

    int getPlayerDirection();
    void spawnArrow();
};