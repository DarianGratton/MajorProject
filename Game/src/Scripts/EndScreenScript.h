#pragma once

#include <entityx/entityx.h>

#include "../CScript.h"

class EndScreenScript : public CScript 
{
public:
    EndScreenScript(Entity* entity);

    void Start() override;
    void Update(TimeDelta dt) override;

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
    void HideEntities(bool active);
};