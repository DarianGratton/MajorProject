#pragma once

#include "../cScript.h"
#include "../scripts.h"
#include "../scriptFactory.h"

struct Script {
    Script(std::string scriptName, entityx::Entity* entity) {

        ScriptFactory scriptFactory;
        script = scriptFactory.getScript(scriptName, entity);

    }

    CScript* script;
};