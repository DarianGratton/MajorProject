#pragma once

#include <entityx/entityx.h>

#include <string>

#include "../Components.h"

using namespace entityx;

struct SceneLoad 
{
    SceneLoad(std::string sceneName, EntityManager* entities) : sceneName(sceneName), entities(entities) {}

    std::string sceneName;
    EntityManager* entities;
};
