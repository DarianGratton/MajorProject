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

private:
    std::string name;
    bool isActive;
};