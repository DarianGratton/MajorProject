#include "ECS.h"

void ECS::init() {
    systems.add<AudioSystem>();
    systems.add<ScriptSystem>();
    systems.add<RenderingSystem>();
    systems.configure();
}

void ECS::update(TimeDelta dt) {
    systems.update<AudioSystem>(dt);
    systems.update<ScriptSystem>(dt);
    systems.update<RenderingSystem>(dt);
}
