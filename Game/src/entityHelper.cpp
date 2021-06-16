#include "entityHelper.h"

#include "components.h"
#include "logger.h"

#include <iostream>
#include <sstream>
#include <iterator>

void EntityHelper::addComponent(Entity* entity, std::string component, std::string parameters) {
    LOG_INFO(component);
    LOG_INFO(parameters);

    // Parse parameters
    std::vector<std::string> values;
    size_t i = 0;
    while ((i = parameters.find(",")) != std::string::npos) {
        values.push_back(parameters.substr(0, i));
        parameters.erase(0, i + 1);
    }
    values.push_back(parameters);

    if (component == "Camera") {
        addCameraComponent(entity, values);
    } else if (component == "Transform") {
        addTransformComponent(entity, values);
    } else if (component == "Script") {
        addScriptComponent(entity, parameters);
    } else if (component == "Shader") {
        addShaderComponent(entity, parameters);
    } else if (component == "Texture") {
        addTextureComponent(entity, parameters);
    } 
}

void EntityHelper::addCameraComponent(Entity* entity, const std::vector<std::string>& parameters) {
    
}

void EntityHelper::addScriptComponent(Entity* entity, const std::string& filepath) {
    
}

void EntityHelper::addShaderComponent(Entity* entity, const std::string& filepath) {
    entity->assign<ShaderComp>(filepath.c_str());
}

void EntityHelper::addTextureComponent(Entity* entity, const std::string& filepath) {
    entity->assign<TextureComp>(filepath.c_str());
}

void EntityHelper::addTransformComponent(Entity* entity, const std::vector<std::string>& parameters) {

}
