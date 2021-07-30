#include "enemyScript.h"

#include "../components.h"
#include "../logger.h"
#include "../input.h"

EnemyScript::EnemyScript(entityx::Entity* entity) : CScript(entity) {
    
}

void EnemyScript::start() {

}

void EnemyScript::update(TimeDelta dt) {
    // Update enemy position
    ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    ComponentHandle<Transform> transform = entity.component<Transform>();
    transform.get()->xpos = rigidBody.get()->body->GetPosition().x;
    transform.get()->ypos = rigidBody.get()->body->GetPosition().y;
}

void EnemyScript::beginContact(Entity* entityA, Entity* entityB) {
    
}

void EnemyScript::endContact(Entity* entityA, Entity* entityB) {

}