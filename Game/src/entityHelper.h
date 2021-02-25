#pragma once

#include <entityx/entityx.h>
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

    // Different components
    void addCameraComponent();
    void addScriptComponent();
    void addShaderComponent();
    void addTextureComponent();
    void addTransformComponent();
};