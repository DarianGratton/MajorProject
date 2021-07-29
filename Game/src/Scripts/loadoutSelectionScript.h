#pragma once

#include <vector>

#include <entityx/entityx.h>

#include "../cScript.h"

class LoadoutSelectionScript : public CScript {
public:
    LoadoutSelectionScript(Entity* entity);

    void start() override;
    void update(TimeDelta dt) override;

private:
    Entity weaponOption1;
    Entity weaponOption2;
    Entity weaponOption3;
    Entity weaponOption4;
    Entity weaponOption5;
    Entity backOption;
    Entity fightOption;
    Entity weaponPreview;
    float arrowPosOffset;
    unsigned int currOption;
    unsigned int numOfOptions;
    float cooldownTimer;

    unsigned int numberSelected;
    bool weaponsSelected[5];
};