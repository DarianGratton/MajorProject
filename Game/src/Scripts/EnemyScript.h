#pragma once

#include <box2d/box2d.h>
#include <entityx/entityx.h>

#include <string>

#include "../CScript.h"

class EnemyScript : public CScript 
{
public:
    EnemyScript(Entity* entity);

    void Start() override;
    void Update(TimeDelta dt) override;
    void BeginContact(Entity* entityA, Entity* entityB) override;
    void EndContact(Entity* entityA, Entity* entityB) override;
    void MoveCharacter(b2Body* body);
    void DamageCharacter(int damage);
    void SetCharacterWeapons(int weapon1Num, int weapon2Num);

    inline void SetCanMove(bool _canMove) { canMove = _canMove; }
    inline void SetIsMovementReduced(bool movementReduced) { isMovementReduced = movementReduced; }
    inline int GetHealth() { return health; }
    inline void SetCharacterAction(int action) { predictedAction = action; }

private:
    Entity enemyHpText;
    Entity weapon1;
    Entity weapon2;
    bool canMove;
    bool isMovementReduced;
    float normalMovementVelocity;
    float reducedMovementVelocity;
    int health;
    int predictedAction;

    std::string GetScriptName(int i);
};