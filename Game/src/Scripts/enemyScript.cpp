#include "enemyScript.h"

#include "../components.h"
#include "../logger.h"
#include "../input.h"

EnemyScript::EnemyScript(entityx::Entity* entity) : CScript(entity) {
    // Initialize variables
    health = 100;
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
    ComponentHandle<Name> entityName = entityB->component<Name>();
    LOG_TRACE(entityName.get()->name);
    if (entityName.get()->name.find("Weapon") != std::string::npos) {

        ComponentHandle<Script> weaponScript = entityB->component<Script>();
        int damage = reinterpret_cast<WeaponScript*>(weaponScript.get()->script)->getDamage();
        health -= damage;
    }
}

void EnemyScript::endContact(Entity* entityA, Entity* entityB) {

}