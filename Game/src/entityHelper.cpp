#include "entityHelper.h"

#include "logger.h"

void EntityHelper::addComponent(Entity* entity, std::string component, std::string parameters) {
    LOG_INFO("EntityHelper Called");
    LOG_INFO(component);
    LOG_INFO(parameters);

    if (component == "Camera") {
        addCameraComponent();
    } else if (component == "Transform") {
        addTransformComponent();
    } else if (component == "Script") {
        addScriptComponent();
    } else if (component == "Shader") {
        addShaderComponent();
    } else if (component == "Texture") {
        addTextureComponent();
    } 
}

void EntityHelper::addCameraComponent() {

}

void EntityHelper::addScriptComponent() {

}

void EntityHelper::addShaderComponent() {

}

void EntityHelper::addTextureComponent() {

}

void EntityHelper::addTransformComponent() {

}
