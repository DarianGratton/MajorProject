#pragma once

#include <string>

#include "../logger.h"

class Weapon {
public:
    Weapon(std::string name) : name(name) {}
    ~Weapon();

    virtual void useWeapon() = 0;
    // Function for collisions

    inline std::string getName() { return name; }
    inline void setIsActive(bool active) { isActive = active; }

     // Collision detection
    virtual void beginContact() { LOG_WARN("Weapon: Abstract class' start contact called."); }
    virtual void endContact() { LOG_WARN("Weapon: Abstract class' end contact called."); }

private:
    std::string name;
    bool isActive;
};