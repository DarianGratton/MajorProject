#include "ECS.h"

#include "systems.h"

void ECS::init() {
    systems.add<AudioSystem>();
    systems.add<ScriptSystem>();
    systems.add<PhysicsSystem>();
    systems.add<RenderingSystem>();
    systems.configure();
}

void ECS::update(TimeDelta dt) {
    systems.update<AudioSystem>(dt);
    systems.update<ScriptSystem>(dt);
    systems.update<PhysicsSystem>(dt);
    systems.update<RenderingSystem>(dt);
}

void ECS::endGame() {
    gameEnded = true;
}

void ECS::pauseGame() {
    gamePaused = true;
}

void ECS::unpauseGame() {
    gamePaused = false;
}

bool ECS::isGamePaused() {
    return gamePaused;
}
