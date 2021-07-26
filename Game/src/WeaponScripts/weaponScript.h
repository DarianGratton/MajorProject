#pragma once

#include <string>

#include "../cScript.h"
#include "../logger.h"

class WeaponScript : CScript {
public:
    WeaponScript(Entity* entity) : CScript(entity) {
        cscript = reinterpret_cast<CScript*>(this);
    }

    virtual void useWeapon() = 0;
    
    inline void setIsActive(bool active) { isActive = active; }
    inline float getDamage() { return damage; }
    inline Entity* getEntity() { return &entity; }

    // Collision detection
    virtual void beginContact(Entity* entityA, Entity* entityB) { LOG_WARN("WeaponScript: Abstract class' start contact called."); }
    virtual void endContact(Entity* entityA, Entity* entityB) { LOG_WARN("WeaponScript: Abstract class' end contact called."); }

protected:
    bool isActive;
    float damage;
    CScript* cscript;
};