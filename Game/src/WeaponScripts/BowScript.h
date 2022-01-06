#pragma once

#include <entityx/entityx.h>
#include <vector>

#include "WeaponScript.h"

class BowScript : public WeaponScript 
{
public:
    BowScript(Entity* entity, float spriteHeight, float spriteWidth);

    void Start() override;
    void Update(TimeDelta dt) override;
    void UseWeapon() override;

    // Collision detection
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

private:
    Entity userEntity;
    float spriteOffset;
    float projectileVelocity;
    float projectileLifespan;
    float fireRate;
    int arrowNumber;
    bool isPlayer;

    // Projectile data structures
    vector<pair<Entity, Direction>> projectiles;
    vector<float> projectilesTimeElapsed;
    vector<string> projectilesFlaggedForDeletion; 

    void SpawnArrow();
};