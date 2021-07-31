#pragma once

#include "entityx/entityx.h"

#include "../cScript.h"

class EnemyScript : public CScript {
public:
    EnemyScript(entityx::Entity* entity);

    void start() override;
    void update(TimeDelta dt) override;
    void beginContact(Entity* entityA, Entity* entityB) override;
    void endContact(Entity* entityA, Entity* entityB) override;

    inline int getHealth() { return health; }

private:
    Entity enemyHpText;
    int health;
};