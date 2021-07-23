#include "entityHelper.h"

#include <glm/glm.hpp>

#include "components.h"
#include "logger.h"

#include <iostream>
#include <sstream>
#include <iterator>

void EntityHelper::addComponent(Entity* entity, std::string component, std::string parameters) {
    // Parse parameters
    std::vector<std::string> values;
    size_t i = 0;
    while ((i = parameters.find(",")) != std::string::npos) {
        values.push_back(parameters.substr(0, i));
        parameters.erase(0, i + 1);
    }
    values.push_back(parameters);

    if (component == "name") {
        addNameComponent(entity, values[0]);
    } else if (component == "Camera") {
        addCameraComponent(entity, values);
    } else if (component == "Transform") {
        addTransformComponent(entity, values);
    } else if (component == "Script") {
        addScriptComponent(entity, values[0]);
    } else if (component == "Shader") {
        addShaderComponent(entity, values[0]);
    } else if (component == "Texture") {
        addTextureComponent(entity, values[0]);
    } else if (component == "TextSprite") {
        addTextComponent(entity, values);
    } else if (component == "Audio") {
        addAudioComponent(entity, values);
    } else if (component == "Script") {
        addScriptComponent(entity, values[0]);
    } else if (component == "RigidBody") {
        addRigidBodyComponent(entity, values);
    }
}

void EntityHelper::addNameComponent(Entity* entity, const std::string& name) {
    entity->assign<Name>(name);
}

// TODO: Possibly clean up this function it's probably effiecent but not very clean
void EntityHelper::addCameraComponent(Entity* entity, const std::vector<std::string>& parameters) {
    float leftf, rightf, bottomf, topf, znear, zfar;

    if (parameters.empty()) {
        entity->assign<Camera>();
        return;
    }

    // 1 parameter
    std::stringstream str(parameters.at(0));
    if (!(str >> leftf)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 1 invalid value.");
        return;
    }
    if (parameters.size() == 1) {
        entity->assign<Camera>(leftf);
        return;
    }

    // 2 parameters
	str = std::stringstream(parameters.at(1));
    if (!(str >> rightf)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 2 invalid value.");
        return;
    }
    if (parameters.size() == 2) {
        entity->assign<Camera>(leftf, rightf);
        return;
    }

    // 3 parameters
	str = std::stringstream(parameters.at(2));
    if (!(str >> bottomf)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 3 invalid value.");
        return;
    }
    if (parameters.size() == 3) {
        entity->assign<Camera>(leftf, rightf, bottomf);
        return;
    }

    // 4 parameters
	str = std::stringstream(parameters.at(3));
    if (!(str >> topf)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 4 invalid value.");
        return;
    }
    if (parameters.size() == 4) {
        entity->assign<Camera>(leftf, rightf, bottomf, topf);
        return;
    }

    // 5 parameters
	str = std::stringstream(parameters.at(4));
    if (!(str >> znear)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 5 invalid value.");
        return;
    }
    if (parameters.size() == 5) {
        entity->assign<Camera>(leftf, rightf, bottomf, topf, znear);
        return;
    }

    // 6 parameters
	str = std::stringstream(parameters.at(5));
    if (!(str >> zfar)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 6 invalid value.");
        return;
    }

    entity->assign<Camera>(leftf, rightf, bottomf, topf, znear, zfar);
}

void EntityHelper::addScriptComponent(Entity* entity, const std::string& scriptName) {
    entity->assign<Script>(scriptName, entity);
}

void EntityHelper::addShaderComponent(Entity* entity, const std::string& filepath) {
    std::string temp = filepath;
    int n = temp.length();
    char *chararray = new char [n+1];
    strcpy(chararray,temp.c_str());

    entity->assign<ShaderComp>(chararray);
}

void EntityHelper::addTextureComponent(Entity* entity, const std::string& filepath) {
    std::string temp = filepath;
    int n = temp.length();
    char *chararray = new char [n+1];
    strcpy(chararray,temp.c_str());

    entity->assign<TextureComp>(chararray);
}

// TODO: Scale should be used
void EntityHelper::addTransformComponent(Entity* entity, const std::vector<std::string>& parameters) {
    float xpos, ypos, zpos;        // Position
    float angle, xrot, yrot, zrot; // Rotation
    float xscl, yscl, zscl;        // Scale

     if (parameters.empty()) {
        entity->assign<Camera>();
        return;
    }

    // 1 parameter
    std::stringstream str(parameters.at(0));
    if (!(str >> xpos)) {
        LOG_ERROR("Scene parsing - addTransform Error: float 1 invalid value.");
        return;
    }
    if (parameters.size() == 1) {
        entity->assign<Transform>(xpos);
        return;
    }

    // 2 parameters
	str = std::stringstream(parameters.at(1));
    if (!(str >> ypos)) {
        LOG_ERROR("Scene parsing - addTransform Error: float 2 invalid value.");
        return;
    }
    if (parameters.size() == 2) {
        entity->assign<Transform>(xpos, ypos);
        return;
    }

    // 3 parameters
	str = std::stringstream(parameters.at(2));
    if (!(str >> zpos)) {
        LOG_ERROR("Scene parsing - addTransform Error: float 3 invalid value.");
        return;
    }
    if (parameters.size() == 3) {
        entity->assign<Transform>(xpos, ypos, zpos);
        return;
    }

    // 4 parameters
	str = std::stringstream(parameters.at(3));
    if (!(str >> angle)) {
        LOG_ERROR("Scene parsing - addTransform Error: float 4 invalid value.");
        return;
    }
    if (parameters.size() == 4) {
        entity->assign<Transform>(xpos, ypos, zpos, angle);
        return;
    }

    // 5 parameters
	str = std::stringstream(parameters.at(4));
    if (!(str >> xrot)) {
        LOG_ERROR("Scene parsing - addTransform Error: float 5 invalid value.");
        return;
    }
    if (parameters.size() == 5) {
        entity->assign<Transform>(xpos, ypos, zpos, angle, xrot);
        return;
    }

    // 6 parameters
	str = std::stringstream(parameters.at(5));
    if (!(str >> yrot)) {
        LOG_ERROR("Scene parsing - addTransform Error: float 6 invalid value.");
        return;
    }
    if (parameters.size() == 6) {
        entity->assign<Transform>(xpos, ypos, zpos, angle, xrot, yrot);
        return;
    }

    // 7 parameters
	str = std::stringstream(parameters.at(6));
    if (!(str >> zrot)) {
        LOG_ERROR("Scene parsing - addTransform Error: float 7 invalid value.");
        return;
    }

    entity->assign<Transform>(xpos, ypos, zpos, angle, xrot, yrot, zrot);
}

void EntityHelper::addAudioComponent(Entity* entity, const std::vector<std::string>& parameters) {
    
    if (parameters.size() == 0) {
        LOG_ERROR("Scene parsing - AddAudioSource Error: no parameters - requires at least 1");
        return;
    }

    if (parameters.size() == 1) {
        entity->assign<Audio>(parameters.at(0));
        return;
    }

    bool isPlayOnLoad;
    std::stringstream str(parameters.at(1));
    if (str.str() == "true" || str.str() == "false") {
        LOG_ERROR("Scene parsing - AddAudioSource Error: boolean 1 was found to be true/false - use 1/0 instead.");
        return;
    } else if (str.str() != "0" && str.str() != "1") {
        LOG_ERROR("Scene parsing - AddAudioSource Error: boolean 1 incorrect value - use 1/0.");
        return;
    }
    str >> isPlayOnLoad;

    if (parameters.size() == 2) {
        entity->assign<Audio>(parameters.at(0), isPlayOnLoad);
        return;
    }

    bool isLoop;
    str = std::stringstream(parameters.at(2));
    if (str.str() == "true" || str.str() == "false") {
        LOG_ERROR("Scene parsing - AddAudioSource Error: boolean 1 was found to be true/false - use 1/0 instead.");
        return;
    } else if (str.str() != "0" && str.str() != "1") {
        LOG_ERROR("Scene parsing - AddAudioSource Error: boolean 1 incorrect value - use 1/0.");
        return;
    }
    str >> isLoop;

    entity->assign<Audio>(parameters.at(0).c_str(), isPlayOnLoad, isLoop);
}

void EntityHelper::addTextComponent(Entity* entity, const std::vector<std::string>& parameters) {
    float r, g, b;
    unsigned int pixelwidth, pixelheight;

    // 1 parameter
    if (parameters.size() == 1) {
        entity->assign<TextSprite>(parameters.at(0));
        return;
    }

    // 4 parameters
    std::stringstream str(parameters.at(1));
    if (!(str >> r)) {
        LOG_ERROR("Scene parsing - addText Error: float 1 invalid value.");
        return;
    }
    str = std::stringstream(parameters.at(2));
    if (!(str >> g)) {
        LOG_ERROR("Scene parsing - addText Error: float 2 invalid value.");
        return;
    }
    str = std::stringstream(parameters.at(3));
    if (!(str >> b)) {
        LOG_ERROR("Scene parsing - addText Error: float 3 invalid value.");
        return;
    }
    if (parameters.size() == 4) {
        entity->assign<TextSprite>(parameters.at(0), glm::vec3(r, g, b));
        return;
    }

    // 5 parameters
    str = std::stringstream(parameters.at(4));
    if (!(str >> pixelwidth)) {
        LOG_ERROR("Scene parsing - addText Error: unsigned int 1 invalid value.");
        return;
    }
    if (parameters.size() == 5) {
        entity->assign<TextSprite>(parameters.at(0), glm::vec3(r, g, b), pixelwidth);
        return;
    }

    // 5 parameters
    str = std::stringstream(parameters.at(5));
    if (!(str >> pixelheight)) {
        LOG_ERROR("Scene parsing - addText Error: unsigned int 2 invalid value.");
        return;
    }

    entity->assign<TextSprite>(parameters.at(0), glm::vec3(r, g, b), pixelwidth, pixelheight);
}

void EntityHelper::addRigidBodyComponent(Entity* entity, const std::vector<std::string>& parameters) {
    float xpos, ypos, hx, hy;
    float density, friction;
    bool isDynamicBody;

    // 4 parameters
    std::stringstream str(parameters.at(0));
    if (!(str >> xpos)) {
        LOG_ERROR("Scene parsing - addRigidBody Error: float 1 invalid value.");
        return;
    }
    str = std::stringstream(parameters.at(1));
    if (!(str >> ypos)) {
        LOG_ERROR("Scene parsing - addRigidBody Error: float 2 invalid value.");
        return;
    }
    str = std::stringstream(parameters.at(2));
    if (!(str >> hx)) {
        LOG_ERROR("Scene parsing - addRigidBody Error: float 3 invalid value.");
        return;
    }
    str = std::stringstream(parameters.at(3));
    if (!(str >> hy)) {
        LOG_ERROR("Scene parsing - addRigidBody Error: float 4 invalid value.");
        return;
    }
    if (parameters.size() == 4) {
        entity->assign<RigidBody>(xpos, ypos, hx, hy);
        return;
    }

    // 5 parameters
    str = std::stringstream(parameters.at(4));
    if (!(str >> density)) {
        LOG_ERROR("Scene parsing - addRigidBody Error: float 5 invalid value.");
        return;
    }
    if (parameters.size() == 5) {
        entity->assign<RigidBody>(xpos, ypos, hx, hy, density);
        return;
    }

    // 6 parameters
    str = std::stringstream(parameters.at(5));
    if (!(str >> friction)) {
        LOG_ERROR("Scene parsing - addRigidBody Error: float 6 invalid value.");
        return;
    }
    if (parameters.size() == 5) {
        entity->assign<RigidBody>(xpos, ypos, hx, hy, density, friction);
        return;
    }

    // 7 parameters
    str = std::stringstream(parameters.at(6));
    if (!(str >> isDynamicBody)) {
        LOG_ERROR("Scene parsing - addRigidBody Error: bool 1 invalid value.");
        return;
    }

    entity->assign<RigidBody>(xpos, ypos, hx, hy, density, friction, isDynamicBody);
}
