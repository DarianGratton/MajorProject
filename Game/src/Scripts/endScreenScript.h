#pragma once

#include "entityx/entityx.h"

#include "../cScript.h"

class EndScreenScript : public CScript {
public:
    EndScreenScript(Entity* entity);

    void start() override;
    void update(TimeDelta dt) override;

private:
    Entity endingText;
    Entity restartText;
    Entity exitText;
    Entity pauseBackground;
    Entity enemyHpText;
    Entity playerHpText;

    Entity player;
    Entity enemy;

    float cooldownTimer;
    unsigned int currOption;
    unsigned int numOfOptions;

    bool isActive;
    void hideEntities(bool active);
};