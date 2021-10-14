#pragma once

#include <entityx/entityx.h>

#include "../CScript.h"

class EnemyScript : public CScript 
{
public:
    EnemyScript(Entity* entity);

    void Start() override;
    void Update(TimeDelta dt) override;
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

    inline int GetHealth() { return health; }

private:
    Entity enemyHpText;
    int health;
};