#pragma once

#include <entityx/entityx.h>

#include "../CScript.h"

class MainMenuScript : public CScript 
{
public:
    MainMenuScript(Entity* entity);

    void Start() override;
    void Update(TimeDelta dt) override;

private:
    Entity startText;
    Entity quitText;
    unsigned int currOption;
    unsigned int numOfOptions;
    float cooldownBetweenKeys;
    float sceneLoadDelay;
};