#pragma once

#include <entityx/entityx.h>
#include <vector>

#include "weaponScript.h"

class GrenadeScript : public WeaponScript {
public:
    GrenadeScript(Entity* entity, float spriteHeight, float spriteWidth);

    void start() override;
    void update(TimeDelta dt) override;
    void useWeapon() override;

    // Collision detection
    void beginContact(Entity* entityA, Entity* entityB) override;
    void endContact(Entity* entityA, Entity* entityB) override;

private:
    Entity player;
    Entity grenadeEntity;
    Entity explosionEntity;
    int directionThrown;
    float spriteOffset;
    float spriteHeight;
    float spriteWidth;
    float projectileSpeed;
    float projectileLifespan;
    float explosionLifespan;
    float cooldownTimer;
    float timeElapsed;

    int getPlayerDirection();
    void spawnGrenade();
    void spawnExplosion();
};