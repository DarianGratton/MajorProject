#include "EnemyScript.h"

#include "../Components.h"
#include "../Logger.h"
#include "../Input.h"
#include "../Game.h"

EnemyScript::EnemyScript(entityx::Entity* entity) : CScript(entity) 
{
    // Initialize variables
    health = 100;
    canMove = true;
    isMovementReduced = false;
    normalMovementVelocity = 125.0f;
    reducedMovementVelocity = 50.0f;
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

    // Movement
    ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    MoveCharacter(rigidBody.get()->body);
}

void EnemyScript::MoveCharacter(b2Body* body)
{
    // Movement
    float desiredVelX = 0;
    float desiredVelY = 0;
    float velcityChange = (isMovementReduced ? reducedMovementVelocity : normalMovementVelocity);

    if (canMove)
    {
        // Movement UP
        if (Input::Instance().IsKeyPressed(GLFW_KEY_UP)) 
        {
            desiredVelY = velcityChange;
        }

        // Movement DOWN
        if (Input::Instance().IsKeyPressed(GLFW_KEY_DOWN)) 
        {
            desiredVelY = -velcityChange;
        }
        
        // Movement RIGHT
        if (Input::Instance().IsKeyPressed(GLFW_KEY_RIGHT)) 
        {
            desiredVelX = velcityChange;
        }
        
        // Movement LEFT
        if (Input::Instance().IsKeyPressed(GLFW_KEY_LEFT)) 
        {
            desiredVelX = -velcityChange;
        }    
    }

    // Apply forces
    b2Vec2 enemyVelocity = body->GetLinearVelocity();
    
    float velChangeX = desiredVelX - enemyVelocity.x;
    float velChangeY = desiredVelY - enemyVelocity.y;
    float impulseX = body->GetMass() * velChangeX;
    float impulseY = body->GetMass() * velChangeY;
    body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), body->GetWorldCenter(), true);

    // Update enemy position
    ComponentHandle<Transform> transform = entity.component<Transform>();
    transform.get()->xpos = body->GetPosition().x;
    transform.get()->ypos = body->GetPosition().y;
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
