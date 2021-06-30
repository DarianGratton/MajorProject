#pragma once

#include <string>

#include <entityx/entityx.h>

#include "cScript.h"
#include "scripts.h"

class ScriptFactory {
public:
    ScriptFactory() {}

    CScript* getScript(std::string scriptName, entityx::Entity* entity);
};
