#pragma once

#include <entityx/entityx.h>

#include <vector>

#include "../CScript.h"

class LoadoutSelectionScript : public CScript 
{
public:
    LoadoutSelectionScript(Entity* entity);

    void Start() override;
    void Update(TimeDelta dt) override;

private:
    Entity weaponOption1;
    Entity weaponOption2;
    Entity weaponOption3;
    Entity weaponOption4;
    Entity weaponOption5;
    Entity backOption;
    Entity fightOption;
    Entity weaponPreview;
    std::vector<Entity> selectionIndicators;
    float arrowPosOffset;
    unsigned int currOption;
    unsigned int numOfOptions;
    float cooldownTimer;

    unsigned int numberSelected;
    bool weaponsSelected[5];
};