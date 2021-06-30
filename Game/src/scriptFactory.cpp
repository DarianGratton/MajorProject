#include "scriptFactory.h"

CScript* ScriptFactory::getScript(std::string scriptName, entityx::Entity* entity) {

    if (scriptName == "PlayerScript") {
        return new PlayerScript(entity);
    } 

    // If scriptName is not found
    return nullptr;

}