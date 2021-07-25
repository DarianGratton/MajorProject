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

    if (scriptName == "SwordScript") {
        return reinterpret_cast<CScript*>(new SwordScript(entity, 10.0f, 10.0f));
    }

    if (scriptName == "BowScript") {
        return reinterpret_cast<CScript*>(new BowScript(entity, 8.0f, 8.0f));
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