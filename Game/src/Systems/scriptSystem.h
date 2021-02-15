#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <entityx/entityx.h>

#include "../Components/script.h"

#include "../logger.h"

using namespace entityx;

class ScriptSystem : public System<ScriptSystem> {
public:
    void update(EntityManager& es, EventManager& events, TimeDelta dt) override {
        auto entities = es.entities_with_components<Script>();

        for (Entity e : entities) {
            ComponentHandle<Script> script = e.component<Script>();
            script.get()->script->update();
        }
    }
};
