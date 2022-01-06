#pragma once

#include <entityx/entityx.h>
#include <vector>

#include "WeaponScript.h"

class GrenadeScript : public WeaponScript 
{
public:
    GrenadeScript(Entity* entity, float spriteHeight, float spriteWidth);

    void Start() override;
    void Update(TimeDelta dt) override;
    void UseWeapon() override;

    // Collision detection
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

private:
    Entity userEntity;
    Entity grenadeEntity;
    Entity explosionEntity;
    Direction directionThrown;

    float spriteOffset;
    float projectileVelocity;
    float projectileLifespan;
    float explosionLifespan;
    float cooldownTimer;
    float timeElapsed;
    bool isPlayer;
    
    void SpawnGrenade();
    void SpawnExplosion();
};