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

    Direction getDirection(int key1, int key2 = 0) 
    {
        if (!key2) 
        {
            if (key1 == GLFW_KEY_UP)
                return NORTH;

            if (key1 == GLFW_KEY_DOWN)
                return SOUTH;

            if (key1 == GLFW_KEY_LEFT)
                return WEST;

            if (key1 == GLFW_KEY_RIGHT)
                return EAST;
        }
        else
        {
            if (key1 == GLFW_KEY_UP && key2 == GLFW_KEY_LEFT)
                return NORTHWEST;

            if (key1 == GLFW_KEY_UP && key2 == GLFW_KEY_RIGHT)
                return NORTHEAST;

            if (key1 == GLFW_KEY_DOWN && key2 == GLFW_KEY_LEFT)
                return SOUTHWEST;

            if (key1 == GLFW_KEY_DOWN && key2 == GLFW_KEY_RIGHT)
                return SOUTHEAST;
        }

        // Otherwise
        return NONE;
    }
};