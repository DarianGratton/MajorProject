#include "enemyScript.h"

#include "../components.h"
#include "../logger.h"
#include "../input.h"

EnemyScript::EnemyScript(entityx::Entity* entity) : CScript(entity) {
    // Initialize variables
    health = 100;
}

void EnemyScript::start() {
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::instance().entities.entities_with_components(entityName)) {

        if (entityName.get()->name == "EnemyHpText")
            enemyHpText = e;
    
    }

    // Display Enemy HP
    if (enemyHpText.valid()) {
        ComponentHandle<TextSprite> textComp = enemyHpText.component<TextSprite>();
        textComp.get()->text = "Enemy HP: " + std::to_string(health); 
    }

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
    if (entityName.get()->name.find("Weapon") != std::string::npos) {

        // Update enemy hp
        ComponentHandle<Script> weaponScript = entityB->component<Script>();
        int damage = reinterpret_cast<WeaponScript*>(weaponScript.get()->script)->getDamage();
        health -= damage;

        // Display Enemy HP
        ComponentHandle<TextSprite> textComp = enemyHpText.component<TextSprite>();
        textComp.get()->text = "Enemy HP: " + std::to_string(health); 
    }
}

void EnemyScript::endContact(Entity* entityA, Entity* entityB) {

}