#include "EnemyScript.h"

#include "../Components.h"
#include "../Logger.h"
#include "../Input.h"
#include "../Game.h"

EnemyScript::EnemyScript(entityx::Entity* entity) : CScript(entity) 
{
    // Initialize variables
    health = 100;
}

void EnemyScript::Start() 
{
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        if (entityName.get()->name == "EnemyHpText")
            enemyHpText = e;
    }

    // Display Enemy HP
    if (enemyHpText.valid()) 
    {
        ComponentHandle<TextSprite> textComp = enemyHpText.component<TextSprite>();
        textComp.get()->text = "Enemy HP: " + std::to_string(health); 
    }
}

void EnemyScript::Update(TimeDelta dt) 
{
    if (Game::Instance().IsGamePaused())
        return; 

    // Update enemy position
    ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    ComponentHandle<Transform> transform = entity.component<Transform>();
    transform.get()->xpos = rigidBody.get()->body->GetPosition().x;
    transform.get()->ypos = rigidBody.get()->body->GetPosition().y;
}

void EnemyScript::BeginContact(Entity* entityA, Entity* entityB) 
{
    ComponentHandle<Name> entityName = entityB->component<Name>();
    if (entityName.get()->name.find("Weapon") != std::string::npos) 
    {
        // Update enemy hp
        ComponentHandle<Script> weaponScript = entityB->component<Script>();
        int damage = reinterpret_cast<WeaponScript*>(weaponScript.get()->script)->GetDamage();
        health -= damage;

        // Display Enemy HP
        ComponentHandle<TextSprite> textComp = enemyHpText.component<TextSprite>();
        textComp.get()->text = "Enemy HP: " + std::to_string(health); 
    }
}

void EnemyScript::EndContact(Entity* entityA, Entity* entityB) {}
