#include "scene.h"

#include <entityx/entityx.h>
#include <tinyxml2.h>

#include "ECS.h"
#include "entityHelper.h"
#include "logger.h"

using namespace tinyxml2;

bool Scene::load() {
    // Create xml object to read from
    TinyXMLDocument* doc = new TinyXMLDocument();
    XMLError xmlerr = doc->LoadFile(filename.c_str());
    if (xmlerr != XML_SUCCESS) {
        LOG_ERROR("Failed to open " + filename + "!");
        return 0;
    }
    XMLElement* root = doc->FirstChildElement("root");

    // Read xml elements
    XMLNode* entity = root->FirstChild();
    while (entity != NULL) {
        XMLNode* component = entity->FirstChild();
        Entity e = ECS::instance().entities.create();

        while (component != NULL) {
            // Error check
            std::string name = component->Value();
            if (name != "Component")  {
                component = component->NextSibling();
                continue;
            }

            // Setup
            const XMLAttribute* attr = component->ToElement()->FirstAttribute();
            std::unordered_map<std::string, std::string> attributes;

            // Get Attributes
            while (attr != NULL) {
                attributes.insert(std::make_pair(attr->Name(), attr->Value()));
                attr = attr->Next();
            }

            // Setup entity
            EntityHelper::instance().addComponent(&e, attributes.at("name"), attributes.at("value"));

            component = component->NextSibling();
        }

        entity = entity->NextSibling();
    }

    return 1;
}