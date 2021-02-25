#include "scene.h"

#include <entityx/entityx.h>
#include <tinyxml2.h>

#include "logger.h"

using namespace tinyxml2;

Scene::Scene(std::string name, std::string filename) {
    
}

bool Scene::load(std::string filename) {
    // // Create xml object to read from
    TinyXMLDocument* doc = new TinyXMLDocument();
    XMLError xmlerr = doc->LoadFile(filename.c_str());

    if (xmlerr != XML_SUCCESS) {
        LOG_ERROR("Failed to open " + filename + "!");
        return 0;
    }
    XMLElement* root = doc->FirstChildElement("root");

    // Read xml elements
    XMLNode* entity = root->FirstChild();
    while (entity != nullptr) {
        XMLNode* component = entity->FirstChild();
        
        while (component != nullptr) {
            component->NextSibling();
        }

        entity = entity->NextSibling();
    }

    return 1;
}