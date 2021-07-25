#pragma once

#include <entityx/entityx.h>

#include "../cScript.h"

class PlayerScript : public CScript {
public:
    PlayerScript(entityx::Entity* entity);

    void start() override;
    void update(TimeDelta dt) override;
    void beginContact() override;
    void endContact() override;

    inline void setCanPlayerMove(bool canMove) { canPlayerMove = canMove; }

private:
    Entity weapon1;
    Entity weapon2; 
    bool canPlayerMove;
};