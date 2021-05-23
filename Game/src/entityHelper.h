// #pragma once

// #include <entityx/entityx.h>
// #include <string>

// using namespace entityx;

// class EntityHelper {
// public:
//     static EntityHelper& instance() {
//         static EntityHelper *instance = new EntityHelper();
//         return *instance;
//     }

//     EntityHelper(EntityHelper const&) = delete;
//     void operator=(EntityHelper const&) = delete;

//     void addComponent(Entity* entity, std::string component, std::string parameters);

// private:
//     EntityHelper() {}

//     // Different components
//     void addCameraComponent(Entity* entity, std::string parameters);
//     void addScriptComponent(Entity* entity, std::string filepath);
//     void addShaderComponent(Entity* entity, std::string filepath);
//     void addTextureComponent(Entity* entity, std::string filepath);
//     void addTransformComponent(Entity* entity, std::string parameters);
// };
