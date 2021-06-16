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
    void addCameraComponent(Entity* entity, const std::vector<std::string>& parameters);
    void addScriptComponent(Entity* entity, const std::string& filepath);
    void addShaderComponent(Entity* entity, const std::string& filepath);
    void addTextureComponent(Entity* entity, const std::string& filepath);
    void addTransformComponent(Entity* entity, const std::vector<std::string>& parameters);
};
