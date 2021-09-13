#pragma once

#include <entityx/entityx.h>

#include "WeaponScript.h"

class SwordScript : public WeaponScript 
{
public:
    SwordScript(Entity* entity, float spriteHeight, float spriteWidth);

    void Start() override;
    void Update(TimeDelta dt) override;
    void UseWeapon() override;

    // Collision detection
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

private:
    Entity player;

    float spriteOffset;   // Sword sprite offset from player sprite
    float attackRate;     // Attack rate of the sword
    float attackCooldown; // Attack cooldown of the sword

    void CreateEntity();
};