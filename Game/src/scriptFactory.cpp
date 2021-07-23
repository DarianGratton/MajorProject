#include "scriptFactory.h"

CScript* ScriptFactory::getScript(std::string scriptName, entityx::Entity* entity) {

    // Characters
    if (scriptName == "PlayerScript") {
        return new PlayerScript(entity);
    }

    if (scriptName == "EnemyScript") {
        return new EnemyScript(entity);
    }

    // Weapons
    if (scriptName == "ShieldScript") {
        return reinterpret_cast<CScript*>(new ShieldScript(entity));
    }

    // Menu Navigation
    if (scriptName == "MainMenuScript") {
        return new MainMenuScript(entity);
    }

    if (scriptName == "LoadoutSelectionScript") {
        return new LoadoutSelectionScript(entity);
    }

    // If scriptName is not found
    return nullptr;

}