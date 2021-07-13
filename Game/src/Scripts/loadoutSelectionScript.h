#pragma once

#include <vector>

#include <entityx/entityx.h>

#include "../cScript.h"

class LoadoutSelectionScript : public CScript {
public:
    LoadoutSelectionScript(Entity* entity);

    void start() override;
    void update() override;

private:
    Entity weaponOption1;
    Entity weaponOption2;
    Entity weaponOption3;
    Entity weaponOption4;
    Entity weaponOption5;
    Entity weaponOption6;
    Entity backOption;
    Entity fightOption;
    unsigned int currOption;
    unsigned int numOfOptions;
};