#pragma once

#include <entityx/entityx.h>

#include <string>

#include "../Components.h"

using namespace std;
using namespace entityx;

struct SceneLoad 
{
    SceneLoad(string sceneName, EntityManager* entities) : sceneName(sceneName), entities(entities) {}

    string sceneName;
    EntityManager* entities;
};
