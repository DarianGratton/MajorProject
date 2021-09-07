#pragma once

#include "../CScript.h"
#include "../Input.h"
#include "../Logger.h"

class WeaponScript : CScript 
{
public:
    WeaponScript(Entity* entity) : CScript(entity) 
    {
        cscript = reinterpret_cast<CScript*>(this);
    }

    virtual void UseWeapon() = 0;
    
    inline void SetIsActive(bool active) { isActive = active; }
    inline int GetDamage() { return damage; }
    inline Entity* GetEntity() { return &entity; }

    // Collision detection
    virtual void BeginContact(Entity* entityA, Entity* entityB) { LOG_WARN("WeaponScript: Abstract class' start contact called."); }
    virtual void EndContact(Entity* entityA, Entity* entityB) { LOG_WARN("WeaponScript: Abstract class' end contact called."); }

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
        bool isUpPressed    = Input::Instance().IsKeyPressed(GLFW_KEY_UP);
        bool isDownPressed  = Input::Instance().IsKeyPressed(GLFW_KEY_DOWN);
        bool isLeftPressed  = Input::Instance().IsKeyPressed(GLFW_KEY_LEFT);
        bool isRightPressed = Input::Instance().IsKeyPressed(GLFW_KEY_RIGHT);
        
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