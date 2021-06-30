#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <entityx/entityx.h>

#include "../components.h"

#include "../logger.h"

using namespace entityx;

class ScriptSystem : public System<ScriptSystem> {
public:
    void update(EntityManager& es, EventManager& events, TimeDelta dt) override {

        es.each<Script>([dt](Entity entity, Script &script) {
            if (script.script != nullptr)
                script.script->update();
        });

    }
};
