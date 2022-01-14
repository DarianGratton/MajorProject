#include "Scene.h"

#include <entityx/entityx.h>
#include <tinyxml2.h>
#include <sstream>

#include "ECS.h"
#include "EntityHelper.h"
#include "Logger.h"

using namespace tinyxml2;

bool Scene::Load()
{
    // Create xml object to read from
    TinyXMLDocument* doc = new TinyXMLDocument();
    XMLError xmlerr = doc->LoadFile(filename.c_str());
    if (xmlerr != XML_SUCCESS) 
    {
        LOG_ERROR("Failed to open " + filename + "!");
        return 0;
    }
    XMLElement* root = doc->FirstChildElement("root");

    // Read xml elements
    XMLNode* entity = root->FirstChild();

    // Get entity components
    while (entity != nullptr) 
    {
        XMLNode* component = entity->FirstChild();
    
        if (component == nullptr)
        {
            entity = entity->NextSibling();
            continue;
        }
        
        Entity e = ECS::Instance().entities.create();

        // Get entity attrbutes
        float width = 0;
        float height = 0;
        const XMLAttribute* attr = entity->ToElement()->FirstAttribute();
        while (attr != NULL) 
        {
            string attrName = attr->Name();
            if (attrName == "name") 
            {   // Name of the sprite
                e.assign<Name>(attr->Value());
            }
            if (attrName == "width") 
            {   // Width of the sprite
                stringstream str(attr->Value());
                str >> width;
            }
            if (attrName == "height") 
            {   // Height of the sprite
                stringstream str(attr->Value());
                str >> height;
            }
            attr = attr->Next();
        }

        // Add spritevertices
        vector<float> spriteVertices =  {
                -width/2, -height/2, 0.0f, 0.0f,
                 width/2, -height/2, 1.0f, 0.0f,
                 width/2,  height/2, 1.0f, 1.0f,
                -width/2,  height/2, 0.0f, 1.0f,
            };
        e.assign<SpriteVertices>(spriteVertices);

        // Add active
        e.assign<Active>(true);

        while (component != nullptr) 
        {
            // Error check
            
            if (string(component->Value()) != "Component")  
            {
                component = component->NextSibling();
                continue;
            }

            // Setup
            const XMLAttribute* attr = component->ToElement()->FirstAttribute();
            unordered_map<std::string, std::string> attributes;
            
            // Get Attributes
            while (attr != NULL) 
            {
                attributes.insert(std::make_pair(attr->Name(), attr->Value()));
                attr = attr->Next();
            }

            if (attributes.find("name") == attributes.end() || attributes.find("value") == attributes.end())
            {
                component = component->NextSibling();
                continue;
            }

            // Setup entity
            EntityHelper::Instance().AddComponent(&e, attributes.at("name"), attributes.at("value"));

            component = component->NextSibling();
        }

        entity = entity->NextSibling();
    }

    return 1;
}