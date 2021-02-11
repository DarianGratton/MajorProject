#include "ECS.h"

void ECS::init() {
    systems.add<RenderingSystem>();
    systems.configure();
}

void ECS::update(TimeDelta dt) {
    systems.update<RenderingSystem>(dt);
}
