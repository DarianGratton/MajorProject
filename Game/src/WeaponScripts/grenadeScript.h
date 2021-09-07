#pragma once

#include <entityx/entityx.h>
#include <vector>

#include "WeaponScript.h"

class GrenadeScript : public WeaponScript {
public:
    GrenadeScript(Entity* entity, float spriteHeight, float spriteWidth);

    void Start() override;
    void Update(TimeDelta dt) override;
    void UseWeapon() override;

    // Collision detection
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

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

    int GetPlayerDirection();
    void SpawnGrenade();
    void SpawnExplosion();
};