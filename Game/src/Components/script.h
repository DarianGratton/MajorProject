#pragma once

#include <entityx/entityx.h>

#include "../CScript.h"
#include "../Scripts.h"
#include "../ScriptFactory.h"
#include "../Logger.h"

using namespace std;

struct Script 
{
    Script(string scriptName, entityx::Entity* entity) 
    {
        ScriptFactory scriptFactory;
        script = scriptFactory.GetScript(scriptName, entity);
    }

    Script(CScript* script) : script(script) {}

    CScript* script;
};