#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <entityx/entityx.h>

#include "../components.h"
#include "../events.h"

#include "../logger.h"

using namespace entityx;

class ScriptSystem : public System<ScriptSystem>, public Receiver<ScriptSystem> {
public:
    void configure(EventManager& events) override {
        events.subscribe<SceneLoad>(*this);
    }

    void update(EntityManager& es, EventManager& events, TimeDelta dt) override {

        es.each<Script>([dt](Entity entity, Script &script) {
            if (script.script != nullptr)
                script.script->update();
        });

    }

    void receive(const SceneLoad& sl) {
        
        for (Entity e : sl.entities) {
            ComponentHandle<Script> scriptComp = e.component<Script>();
            scriptComp.get()->script->start();
        }

    }
};
