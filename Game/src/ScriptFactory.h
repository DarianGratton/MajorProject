#pragma once

#include <entityx/entityx.h>

#include <string>

#include "CScript.h"
#include "Scripts.h"

using namespace std;
using namespace entityx;

class ScriptFactory 
{
public:
    ScriptFactory() {}

    CScript* GetScript(string scriptName, Entity* entity);
};
