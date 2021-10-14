#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <entityx/entityx.h>

#include "../Components.h"
#include "../Events.h"
#include "../Logger.h"

using namespace entityx;

class ScriptSystem : public System<ScriptSystem>, public Receiver<ScriptSystem> 
{
public:
    void configure(EventManager& events) override 
    {
        events.subscribe<SceneLoad>(*this);
    }

    void update(EntityManager& es, EventManager& events, TimeDelta dt) override 
    {
        es.each<Script>([dt](Entity entity, Script &script) {
            if (script.script != nullptr)
                script.script->Update(dt);
        });

    }

    void receive(const SceneLoad& sl)
    {
        ComponentHandle<Script> scriptComp;
        for (Entity entity : sl.entities->entities_with_components(scriptComp)) 
        {
            scriptComp = entity.component<Script>();
            scriptComp.get()->script->Start();
        }

    }
};
