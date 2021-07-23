#pragma once

#include "entityx/entityx.h"

#include "../cScript.h"

class PlayerScript : public CScript {
public:
    PlayerScript(entityx::Entity* entity);

    void start() override;
    void update() override;
    void beginContact() override;
    void endContact() override;
};