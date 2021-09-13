#pragma once

#include <entityx/entityx.h>
#include <box2d/box2d.h>

#include "WeaponScript.h"

class ShieldScript : public WeaponScript 
{
public:
    ShieldScript(entityx::Entity* entity, float spriteHeight, float spriteWidth);

    void Start() override;
    void Update(TimeDelta dt) override;
    void UseWeapon() override;

    // Collision detection
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

private:
    Entity player;
    b2Vec2 hitVelocity;
    float shieldCooldown;
};