#include "enemyScript.h"

#include "../components.h"
#include "../logger.h"
#include "../input.h"

EnemyScript::EnemyScript(entityx::Entity* entity) : CScript(entity) {
    
}

void EnemyScript::start() {
     // Set up collisions
    // ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    // rigidBody.get()->body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entity);

}

void EnemyScript::update(TimeDelta dt) {

}

void EnemyScript::beginContact(Entity* entityA, Entity* entityB) {
    
}

void EnemyScript::endContact(Entity* entityA, Entity* entityB) {

}