#pragma once

#include "entityx/entityx.h"

#include "../cScript.h"
#include "../scripts.h"
#include "../scriptFactory.h"
#include "../logger.h"

struct Script {
    Script(std::string scriptName, entityx::Entity* entity) {

        ScriptFactory scriptFactory;
        script = scriptFactory.getScript(scriptName, entity);
    
    }

    CScript* script;
};