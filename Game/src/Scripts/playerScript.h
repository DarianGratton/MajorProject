#pragma once

#include <entityx/entityx.h>

#include <string>

#include "../CScript.h"

class PlayerScript : public CScript 
{
public:
    PlayerScript(Entity* entity);

    void Start() override;
    void Update(TimeDelta dt) override;
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;

    inline void SetCanPlayerMove(bool canMove) { canPlayerMove = canMove; }
    inline int GetHealth() { return health; }

private:
    Entity weapon1;
    Entity weapon2; 
    Entity playerHpText;
    bool canPlayerMove;
    int health;

    string GetScriptName(int i);
};