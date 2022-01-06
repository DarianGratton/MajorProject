#pragma once

#include <entityx/entityx.h>
#include <vector>

#include "WeaponScript.h"

class GunScript : public WeaponScript 
{
public:
    GunScript(Entity* entity, float spriteHeight, float spriteWidth);

    void Start() override;
    void Update(TimeDelta dt) override;
    void UseWeapon() override;

    // Collision detection
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

private:
    Entity userEntity;
    Entity bulletEntity;
    Direction directionThrown;

    float spriteOffset;
    float projectileVelocity;
    float cooldownTimer;
    float timeElapsed;
    bool flagForDeletion;
    bool isPlayer;
    
    void SpawnBullet();
};