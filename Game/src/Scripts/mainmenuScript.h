#pragma once

#include "entityx/entityx.h"

#include "../cScript.h"

class MainMenuScript : public CScript {
public:
    MainMenuScript(Entity* entity);

    void start() override;
    void update() override;

private:
    Entity startText;
    Entity quitText;
    unsigned int currOption;
    unsigned int numOfOptions;
};