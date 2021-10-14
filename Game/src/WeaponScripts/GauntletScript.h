#pragma once

#include <entityx/entityx.h>

#include "WeaponScript.h"

class GauntletScript : public WeaponScript 
{
public:
    GauntletScript(Entity* entity, float spriteHeight, float spriteWidth);

    void Start() override;
    void Update(TimeDelta dt) override;
    void UseWeapon() override;

    // Collision detection
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

private:
    Entity player;
    float spriteOffset;
    float attackRate;

    int GetPlayerDirection();
};