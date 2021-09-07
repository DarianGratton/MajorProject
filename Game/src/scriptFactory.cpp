#include "ScriptFactory.h"

CScript* ScriptFactory::GetScript(std::string scriptName, entityx::Entity* entity) 
{
    // Characters
    if (scriptName == "PlayerScript") 
        return new PlayerScript(entity);

    if (scriptName == "EnemyScript") 
        return new EnemyScript(entity);

    // Weapons
    if (scriptName == "ShieldScript") 
        return reinterpret_cast<CScript*>(new ShieldScript(entity, 15.0f, 15.0f));

    if (scriptName == "SwordScript") 
        return reinterpret_cast<CScript*>(new SwordScript(entity, 10.0f, 10.0f));

    if (scriptName == "BowScript") 
        return reinterpret_cast<CScript*>(new BowScript(entity, 9.0f, 9.0f));

    if (scriptName == "GrenadeScript") 
        return reinterpret_cast<CScript*>(new GrenadeScript(entity, 8.0f, 8.0f));

    if (scriptName == "GauntletScript") 
        return reinterpret_cast<CScript*>(new GauntletScript(entity, 8.0f, 8.0f));

    // Menu Navigation
    if (scriptName == "MainMenuScript") 
        return new MainMenuScript(entity);

    if (scriptName == "LoadoutSelectionScript")
        return new LoadoutSelectionScript(entity);

    if (scriptName == "PauseMenuScript") 
        return new PauseMenuScript(entity);

    if (scriptName == "EndScreenScript") 
        return new EndScreenScript(entity);

    // If scriptName is not found
    return nullptr;

}