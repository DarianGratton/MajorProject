#include "scene.h"

#include <entityx/entityx.h>
#include <tinyxml2.h>
#include <sstream>

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

    // Get entity components
    while (entity != NULL) {
        XMLNode* component = entity->FirstChild();
        Entity e = ECS::instance().entities.create();

        // Get entity attrbutes
        float width, height;
        const XMLAttribute* attr = entity->ToElement()->FirstAttribute();
        while (attr != NULL) {
            std::string attrName = attr->Name();
            if (attrName == "name") {   // Name of the sprite
                e.assign<Name>(attr->Value());
            }
            if (attrName == "width") {  // Width of the sprite
                std::stringstream str(attr->Value());
                str >> width;
            }
            if (attrName == "height") { // Height of the sprite
                std::stringstream str(attr->Value());
                str >> height;
            }
            attr = attr->Next();
        }

        // Add spritevertices
        std::vector<float> spriteVertices =  {
                -width/2, -height/2, 0.0f, 0.0f,
                 width/2, -height/2, 1.0f, 0.0f,
                 width/2,  height/2, 1.0f, 1.0f,
                -width/2,  height/2, 0.0f, 1.0f,
            };
        e.assign<SpriteVertices>(spriteVertices);

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