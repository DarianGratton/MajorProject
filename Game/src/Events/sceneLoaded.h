#pragma once

#include <entityx/entityx.h>
#include <string>

#include "../components.h"

struct SceneLoad {
    SceneLoad(std::string name, entityx::EntityManager::View<Script> entities) : sceneName(sceneName), entities(entities) {}

    std::string sceneName;
    entityx::EntityManager::View<Script> entities;
};