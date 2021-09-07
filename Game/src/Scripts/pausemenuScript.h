#pragma once

#include <entityx/entityx.h>

#include "../CScript.h"

class PauseMenuScript : public CScript
{
public:
    PauseMenuScript(Entity* entity);

    void Start() override;
    void Update(TimeDelta dt) override;

private:
    Entity pauseText;
    Entity resumeText;
    Entity exitText;
    Entity pauseBackground;
    Entity enemyHpText;
    Entity playerHpText;

    float cooldownTimer;
    unsigned int currOption;
    unsigned int numOfOptions;

    bool isActive;
    void HideEntities(bool active);
};