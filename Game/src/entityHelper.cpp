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

    if (component == "name") {
        addNameComponent(entity, values[0]);
    } else if (component == "Camera") {
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

void EntityHelper::addScriptComponent(Entity* entity, const std::string& filepath) {
    
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
        LOG_ERROR("Scene parsing - addCamera Error: float 1 invalid value.");
        return;
    }
    if (parameters.size() == 1) {
        entity->assign<Transform>(xpos);
        return;
    }

    // 2 parameters
	str = std::stringstream(parameters.at(1));
    if (!(str >> ypos)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 2 invalid value.");
        return;
    }
    if (parameters.size() == 2) {
        entity->assign<Transform>(xpos, ypos);
        return;
    }

    // 3 parameters
	str = std::stringstream(parameters.at(2));
    if (!(str >> zpos)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 3 invalid value.");
        return;
    }
    if (parameters.size() == 3) {
        entity->assign<Transform>(xpos, ypos, zpos);
        return;
    }

    // 4 parameters
	str = std::stringstream(parameters.at(3));
    if (!(str >> angle)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 4 invalid value.");
        return;
    }
    if (parameters.size() == 4) {
        entity->assign<Transform>(xpos, ypos, zpos, angle);
        return;
    }

    // 5 parameters
	str = std::stringstream(parameters.at(4));
    if (!(str >> xrot)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 5 invalid value.");
        return;
    }
    if (parameters.size() == 5) {
        entity->assign<Transform>(xpos, ypos, zpos, angle, xrot);
        return;
    }

    // 6 parameters
	str = std::stringstream(parameters.at(5));
    if (!(str >> yrot)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 6 invalid value.");
        return;
    }
    if (parameters.size() == 6) {
        entity->assign<Transform>(xpos, ypos, zpos, angle, xrot, yrot);
        return;
    }

    // 7 parameters
	str = std::stringstream(parameters.at(6));
    if (!(str >> zrot)) {
        LOG_ERROR("Scene parsing - addCamera Error: float 7 invalid value.");
        return;
    }

    entity->assign<Transform>(xpos, ypos, zpos, angle, xrot, yrot, zrot);
}

void EntityHelper::addAudioComponent(Entity* entity, const std::vector<std::string>& paramters) {

}
