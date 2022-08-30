#pragma once

#include <entityx/entityx.h>

#include <vector>
#include <string>

using namespace entityx;

class EntityHelper 
{
public:
    static EntityHelper& Instance() 
    {
        static EntityHelper *instance = new EntityHelper();
        return *instance;
    }

    EntityHelper(EntityHelper const&) = delete;
    void operator=(EntityHelper const&) = delete;

    void AddComponent(Entity* entity, std::string component, std::string parameters);

private:
    EntityHelper() {}

    // Different components;
    void AddCameraComponent(Entity* entity, const std::vector<std::string>& parameters);
    void AddShaderComponent(Entity* entity, const std::string& filepath);
    void AddTextureComponent(Entity* entity, const std::string& filepath);
    void AddTransformComponent(Entity* entity, const std::vector<std::string>& parameters);
    void AddAudioComponent(Entity* entity, const std::vector<std::string>& parameters);
    void AddTextComponent(Entity* entity, const std::vector<std::string>& parameters);
    void AddScriptComponent(Entity* entity, const std::string& scriptName);
    void AddRigidBodyComponent(Entity* entity, const std::vector<std::string>& parameters);
    void AddTempEnemyWeaponsComponent(Entity* entity, const std::vector<std::string>& parameters);
};
