#pragma once

#include "entityx/entityx.h"

#include "../cScript.h"

class PauseMenuScript : public CScript {
public:
    PauseMenuScript(Entity* entity);

    void start() override;
    void update(TimeDelta dt) override;

private:
    Entity pauseText;
    Entity resumeText;
    Entity endMatchText;
    Entity pauseBackground;
    Entity enemyHpText;
    Entity playerHpText;

    float cooldownTimer;
    unsigned int currOption;
    unsigned int numOfOptions;

    bool isActive;
    void hideEntities(bool active);
};