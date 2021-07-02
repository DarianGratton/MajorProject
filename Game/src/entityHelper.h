#pragma once

#include <entityx/entityx.h>

#include <vector>
#include <string>

using namespace entityx;

class EntityHelper {
public:
    static EntityHelper& instance() {
        static EntityHelper *instance = new EntityHelper();
        return *instance;
    }

    EntityHelper(EntityHelper const&) = delete;
    void operator=(EntityHelper const&) = delete;

    void addComponent(Entity* entity, std::string component, std::string parameters);

private:
    EntityHelper() {}

    // Different components;
    void addNameComponent(Entity* entity, const std::string& name);
    void addCameraComponent(Entity* entity, const std::vector<std::string>& parameters);
    void addShaderComponent(Entity* entity, const std::string& filepath);
    void addTextureComponent(Entity* entity, const std::string& filepath);
    void addTransformComponent(Entity* entity, const std::vector<std::string>& parameters);
    void addAudioComponent(Entity* entity, const std::vector<std::string>& parameters);
    void addTextComponent(Entity* entity, const std::vector<std::string>& parameters);
    void addScriptComponent(Entity* entity, const std::string& scriptName);
};
