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

        if (entityName.get()->name == "EnemyWeapon1")
            weapon1 = e;

        if (entityName.get()->name == "EnemyWeapon2")
            weapon2 = e;
    }

    // Display Enemy HP
    if (enemyHpText.valid()) 
    {
        ComponentHandle<TextSprite> textComp = enemyHpText.component<TextSprite>();
        textComp.get()->text = "Enemy HP: " + std::to_string(health); 
    }

    // Note: should be calling start but for some reason it does it for me, something weird with how entityx does for loops.
    if (weapon1.valid() && !weapon1.has_component<Script>()) 
    {
        ComponentHandle<TempEnemyWeapons> weapons = weapon1.component<TempEnemyWeapons>();
        string scriptName = GetScriptName(weapons.get()->weapon1);
        weapon1.assign<Script>(scriptName, &weapon1);
    }

    if (weapon2.valid() && !weapon2.has_component<Script>()) 
    {
        ComponentHandle<TempEnemyWeapons> weapons = weapon1.component<TempEnemyWeapons>();
        string scriptName = GetScriptName(weapons.get()->weapon2);
        weapon2.assign<Script>(scriptName, &weapon2);
    }
}

void EnemyScript::Update(TimeDelta dt) 
{
    if (Game::Instance().IsGamePaused())
        return; 

    // Movement
    ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    MoveCharacter(rigidBody.get()->body);

    // Attack
    // Use weapon 1
    if (weapon1.valid()) 
    {
        if (Input::Instance().IsKeyPressed(GLFW_KEY_O)) 
        {
            ComponentHandle<Script> scriptComp = weapon1.component<Script>();
            WeaponScript* weaponScript = reinterpret_cast<WeaponScript*>(scriptComp.get()->script);
            if (weaponScript)
                weaponScript->UseWeapon();
        } 
        else 
        {
            ComponentHandle<Script> scriptComp = weapon1.component<Script>();
            WeaponScript* weaponScript = reinterpret_cast<WeaponScript*>(scriptComp.get()->script);
            if (weaponScript) 
                weaponScript->SetIsActive(false);
        }
   
    }

    // Use weapon 2
    if (weapon2.valid()) 
    {
        if (Input::Instance().IsKeyPressed(GLFW_KEY_P)) {
            ComponentHandle<Script> scriptComp = weapon2.component<Script>();
            WeaponScript* weaponScript = reinterpret_cast<WeaponScript*>(scriptComp.get()->script);
            if (weaponScript)
                weaponScript->UseWeapon();
        } 
        else 
        {
            ComponentHandle<Script> scriptComp = weapon2.component<Script>();
            WeaponScript* weaponScript = reinterpret_cast<WeaponScript*>(scriptComp.get()->script);
            if (weaponScript) 
                weaponScript->SetIsActive(false);         
        }
    }
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

string EnemyScript::GetScriptName(int i) 
{
    switch(i) 
    {
        case 1:
            return "SwordScript";
        case 2:
            return "ShieldScript";
        case 3:
            return "BowScript";
        case 4:
            return "GrenadeScript";
        case 5:
            return "GunScript";
        default:
            return "SwordScript";
    }
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
