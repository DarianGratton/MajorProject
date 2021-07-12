#include "scriptFactory.h"

CScript* ScriptFactory::getScript(std::string scriptName, entityx::Entity* entity) {

    if (scriptName == "PlayerScript") {
        return new PlayerScript(entity);
    }

    if (scriptName == "MainMenuScript") {
        return new MainMenuScript(entity);
    }

    // If scriptName is not found
    return nullptr;

}