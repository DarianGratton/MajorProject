#pragma once

#include <string>

#include "../cScript.h"
#include "../logger.h"

class WeaponScript : CScript {
public:
    WeaponScript(entityx::Entity* entity) : CScript(entity) {}

    virtual void useWeapon() = 0;
    
    inline void setIsActive(bool active) { isActive = active; }

     // Collision detection
    virtual void beginContact() { LOG_WARN("WeaponScript: Abstract class' start contact called."); }
    virtual void endContact() { LOG_WARN("WeaponScript: Abstract class' end contact called."); }

private:
    bool isActive;
    float damage;
};