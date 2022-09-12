#include "PlayerScript.h"

#include "../PlayerPrefs.h"
#include "../ScriptFactory.h"
#include "../Components.h"
#include "../Logger.h"
#include "../Input.h"
#include "../Game.h"

PlayerScript::PlayerScript(Entity* entity) : CScript(entity) 
{
    // Initialize variables
    health = 100;
    canMove = true;
    isMovementReduced = false;
    normalMovementVelocity = 125.0f;
    reducedMovementVelocity = 50.0f;
}

void PlayerScript::Start() 
{
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::Instance().entities.entities_with_components(entityName)) 
    {       
        entityName = e.component<Name>();
        if (entityName.get()->name == "PlayerWeapon1")
            weapon1 = e;

        if (entityName.get()->name == "PlayerWeapon2")
            weapon2 = e;

        if (entityName.get()->name == "PlayerHpText")
            playerHpText = e;
    }

    if (!Game::Instance().IsAutomaticallyTraining())
    {
        // Note: Should be calling start but for some reason it does it for me, 
        //       something weird with how entityx does for loops.
        if (weapon1.valid() && !weapon1.has_component<Script>())
        {
            std::string scriptName = GetScriptName(PlayerPrefs::Instance().GetWeapon1());
            weapon1.assign<Script>(scriptName, &weapon1);
        }

        if (weapon2.valid() && !weapon2.has_component<Script>())
        {
            std::string scriptName = GetScriptName(PlayerPrefs::Instance().GetWeapon2());
            weapon2.assign<Script>(scriptName, &weapon2);
        }
    }

    // Display Player HP
    if (playerHpText.valid()) 
    {
        ComponentHandle<TextSprite> textComp = playerHpText.component<TextSprite>();
        textComp.get()->text = "Player HP: " + std::to_string(health);
    }
}

void PlayerScript::Update(TimeDelta dt) 
{
    // Don't want to play if game is paused
    if (Game::Instance().IsGamePaused())
        return;

    // Movement
    ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    MoveCharacter(rigidBody.get()->body);
    
    // Attack
    // Use weapon 1
    if (weapon1.valid()) 
    {
        if (Input::Instance().IsKeyPressed(GLFW_KEY_K)) 
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
        if (Input::Instance().IsKeyPressed(GLFW_KEY_L)) {
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

void PlayerScript::MoveCharacter(b2Body* body)
{
    // Movement
    float desiredVelX = 0;
    float desiredVelY = 0;
    float velcityChange = (isMovementReduced ? reducedMovementVelocity : normalMovementVelocity);

    if (canMove)
    {
        // Movement UP
        if (Input::Instance().IsKeyPressed(GLFW_KEY_W)) 
        {
            desiredVelY = velcityChange;

            ComponentHandle<TextureComp> textureComp = entity.component<TextureComp>();
            textureComp.get()->SetTexture("src/Assets/textures/PlayerUp.png");
        }

        // Movement DOWN
        if (Input::Instance().IsKeyPressed(GLFW_KEY_S)) 
        {
            desiredVelY = -velcityChange;

            ComponentHandle<TextureComp> textureComp = entity.component<TextureComp>();
            textureComp.get()->SetTexture("src/Assets/textures/PlayerDown.png");
        }
        
        // Movement RIGHT
        if (Input::Instance().IsKeyPressed(GLFW_KEY_D)) 
        {
            desiredVelX = velcityChange;

            ComponentHandle<TextureComp> textureComp = entity.component<TextureComp>();
            textureComp.get()->SetTexture("src/Assets/textures/PlayerRight.png");
        }
        
        // Movement LEFT
        if (Input::Instance().IsKeyPressed(GLFW_KEY_A)) 
        {
            desiredVelX = -velcityChange;

            ComponentHandle<TextureComp> textureComp = entity.component<TextureComp>();
            textureComp.get()->SetTexture("src/Assets/textures/PlayerLeft.png");
        }    
    }

    // Apply forces
    b2Vec2 playerVelocity = body->GetLinearVelocity();
    
    float velChangeX = desiredVelX - playerVelocity.x;
    float velChangeY = desiredVelY - playerVelocity.y;
    float impulseX = body->GetMass() * velChangeX;
    float impulseY = body->GetMass() * velChangeY;
    body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), body->GetWorldCenter(), true);

    // Update player position
    ComponentHandle<Transform> transform = entity.component<Transform>();
    transform.get()->xpos = body->GetPosition().x;
    transform.get()->ypos = body->GetPosition().y;
}

void PlayerScript::DamageCharacter(int damage)
{
    // Update HP
    health -= damage;

    // Display updated Player HP
    ComponentHandle<TextSprite> textComp = playerHpText.component<TextSprite>();
    textComp.get()->text = "Player HP: " + std::to_string(health);
}

void PlayerScript::SetCharacterWeapons(int weapon1Num, int weapon2Num)
{
    // Note: Start only needs to be called when script is added after the script
    //       system runs it recieve function. In this case it's being set by 
    //       learning agent system so Start() needs to be called.
    if (weapon1.valid() && !weapon1.has_component<Script>())
    {
        std::string scriptName = GetScriptName(weapon1Num);
        weapon1.assign<Script>(scriptName, &weapon1);
        ComponentHandle<Script> scriptComp = weapon1.component<Script>();
        scriptComp.get()->script->Start();
    }

    if (weapon2.valid() && !weapon2.has_component<Script>())
    {
        std::string scriptName = GetScriptName(weapon2Num);
        weapon2.assign<Script>(scriptName, &weapon2);
        ComponentHandle<Script> scriptComp = weapon2.component<Script>();
        scriptComp.get()->script->Start();
    }
}

std::string PlayerScript::GetScriptName(int i)
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

void PlayerScript::BeginContact(Entity* entityA, Entity* entityB) 
{
    ComponentHandle<Name> entityName = entityB->component<Name>();
    if (entityName.get()->name.find("Weapon") != std::string::npos)
    {
        // Update player hp
        ComponentHandle<Script> weaponScript = entityB->component<Script>();
        int damage = reinterpret_cast<WeaponScript*>(weaponScript.get()->script)->GetDamage();
        DamageCharacter(damage);
    }
}

void PlayerScript::EndContact(Entity* entityA, Entity* entityB) { }
