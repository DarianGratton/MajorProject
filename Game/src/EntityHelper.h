#pragma once

#include <entityx/entityx.h>

#include <vector>
#include <string>

using namespace std;
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

    void AddComponent(Entity* entity, string component, string parameters);

private:
    EntityHelper() {}

    // Different components;
    void AddNameComponent(Entity* entity, const string& name);
    void AddCameraComponent(Entity* entity, const vector<string>& parameters);
    void AddShaderComponent(Entity* entity, const string& filepath);
    void AddTextureComponent(Entity* entity, const string& filepath);
    void AddTransformComponent(Entity* entity, const vector<string>& parameters);
    void AddAudioComponent(Entity* entity, const vector<string>& parameters);
    void AddTextComponent(Entity* entity, const vector<string>& parameters);
    void AddScriptComponent(Entity* entity, const string& scriptName);
    void AddRigidBodyComponent(Entity* entity, const vector<string>& parameters);
};
