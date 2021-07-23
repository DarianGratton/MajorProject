#pragma once

#include <entityx/entityx.h>

#include "weaponScript.h"

class ShieldScript : public WeaponScript {
public:
    ShieldScript(entityx::Entity* entity);

    void start() override;
    void update() override;
    void useWeapon() override;

    // Collision detection
    void beginContact() override;
    void endContact() override;
};