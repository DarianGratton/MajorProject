#pragma once

#include <string>

#include "../cScript.h"
#include "../input.h"
#include "../logger.h"

class WeaponScript : CScript {
public:
    WeaponScript(Entity* entity) : CScript(entity) {
        cscript = reinterpret_cast<CScript*>(this);
    }

    virtual void useWeapon() = 0;
    
    inline void setIsActive(bool active) { isActive = active; }
    inline int getDamage() { return damage; }
    inline Entity* getEntity() { return &entity; }

    // Collision detection
    virtual void beginContact(Entity* entityA, Entity* entityB) { LOG_WARN("WeaponScript: Abstract class' start contact called."); }
    virtual void endContact(Entity* entityA, Entity* entityB) { LOG_WARN("WeaponScript: Abstract class' end contact called."); }

protected:
    bool isActive;
    int damage;
    CScript* cscript;

    enum Direction {
        NONE,
        NORTH,
        NORTHEAST,
        EAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        WEST,
        NORTHWEST
    };

    Direction getDirection() 
    {
        bool isUpPressed    = Input::instance().isKeyPressed(GLFW_KEY_UP);
        bool isDownPressed  = Input::instance().isKeyPressed(GLFW_KEY_DOWN);
        bool isLeftPressed  = Input::instance().isKeyPressed(GLFW_KEY_LEFT);
        bool isRightPressed = Input::instance().isKeyPressed(GLFW_KEY_RIGHT);
        
        if (isUpPressed && isLeftPressed)
            return NORTHWEST;

        if (isUpPressed && isRightPressed)
            return NORTHEAST;

        if (isDownPressed && isLeftPressed)
            return SOUTHWEST;

        if (isDownPressed && isRightPressed)
            return SOUTHEAST;

        if (isUpPressed)
            return NORTH;

        if (isDownPressed)
            return SOUTH;

        if (isLeftPressed)
            return WEST;

        if (isRightPressed)
            return EAST;

        // Otherwise
        return NONE;
    }
};