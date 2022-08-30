#pragma once

#include "../CScript.h"
#include "../Components/Texture.h"
#include "../Input.h"
#include "../Logger.h"

class WeaponScript : CScript 
{
public:
    WeaponScript(Entity* entity, float spriteHeight, float spriteWidth) : 
                 CScript(entity), spriteHeight(spriteHeight), spriteWidth(spriteWidth) 
    {
        cscript = reinterpret_cast<CScript*>(this);
    }

    virtual void UseWeapon() = 0;
    
    inline void SetIsActive(bool active) { isActive = active; }

    inline int GetDamage() { return damage; }
    inline Entity* GetEntity() { return &entity; }
    inline bool CanDamageShield() { return canDamageShield; }

    // Collision detection
    virtual void BeginContact(Entity* entityA, Entity* entityB) { LOG_WARN("WeaponScript: Abstract class' start contact called."); }
    virtual void EndContact(Entity* entityA, Entity* entityB) { LOG_WARN("WeaponScript: Abstract class' end contact called."); }

protected:
    bool isActive;
    int damage; 
    float spriteHeight;
    float spriteWidth; 
    bool canDamageShield;
    CScript* cscript;

    enum Direction {
        NONE,
        NORTH,
        EAST,
        SOUTH,
        WEST,
    };

    Direction GetDirection(Entity* e) 
    {
        if (!(e->has_component<TextureComp>()))
            return NONE;

        ComponentHandle<TextureComp> playerTexture = e->component<TextureComp>();
        if (playerTexture.get()->filename.find("Up") != std::string::npos)
            return NORTH;

        if (playerTexture.get()->filename.find("Down") != std::string::npos)
            return SOUTH;

        if (playerTexture.get()->filename.find("Right") != std::string::npos)
            return EAST;

        if (playerTexture.get()->filename.find("Left") != std::string::npos)
            return WEST;

        // No direction found
        return NONE;
    }
};