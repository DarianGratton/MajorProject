#pragma once

#include "entityx/entityx.h"

#include "../cScript.h"

class EnemyScript : public CScript {
public:
    EnemyScript(entityx::Entity* entity);

    void start() override;
    void update(TimeDelta dt) override;
};