#include "SwordScript.h"

#include "../Physics/PhysicsManager.h"
#include "../Components.h"
#include "../Scripts.h"
#include "../ECS.h"

SwordScript::SwordScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity, spriteHeight, spriteWidth)
{
    // Parent class variables
    damage = 10;
    isActive = false;
    canDamageShield = true;

    // Member variables
    spriteOffset = 10.5f;
    attackRate = 0.1f;
    attackCooldown = 0.0f;
}

void SwordScript::Start() 
{
    // Get name of entity for later
    ComponentHandle<Name> weapon = GetEntity()->component<Name>();
    string weaponName = weapon->name;

    // Get reference to entity
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        entityName = e.component<Name>();
        if (entityName.get()->name == "Player" && weaponName.find("Player") != string::npos)
        {
            userEntity = e;
            isPlayer = true;
        }

        if (entityName.get()->name == "Enemy" && weaponName.find("Enemy") != string::npos)
            userEntity = e;
    }


    // Create and setup the sword enitity
    CreateEntity();
}

void SwordScript::Update(TimeDelta dt) 
{
    ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
    if (entityBody.get()->body->IsEnabled()) 
    {
        attackCooldown -= dt;
        if (attackCooldown > 0) 
            return;

        // Disable collision
        entityBody.get()->body->SetEnabled(false);

        // Disable entity
        ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
        activeComp.get()->isActive = false;

        // Enable entity movement
        ComponentHandle<Script> entityScript = userEntity.component<Script>(); 
        if (isPlayer)
            reinterpret_cast<PlayerScript*>(entityScript.get()->script)->SetCanMove(true);
        else
            reinterpret_cast<EnemyScript*>(entityScript.get()->script)->SetCanMove(true);
    }
}

void SwordScript::UseWeapon() 
{
    if (isActive)
        return;

    isActive = true;
    attackCooldown = attackRate;

    // Update sword's transform
    ComponentHandle<Transform> weaponTransform = GetEntity()->component<Transform>();
    ComponentHandle<Transform> entityTransform = userEntity.component<Transform>(); 

    // Determine the direction to use the weapon
    Direction entityDirection = GetDirection(&userEntity);
    if (entityDirection == NORTH) 
    {
        weaponTransform.get()->xpos = entityTransform.get()->xpos;
        weaponTransform.get()->ypos = entityTransform.get()->ypos + spriteOffset; 
    
    }
    else if (entityDirection == SOUTH) 
    {
        weaponTransform.get()->xpos = entityTransform.get()->xpos;
        weaponTransform.get()->ypos = entityTransform.get()->ypos - spriteOffset; 
    
    }
    else if (entityDirection == EAST) 
    {
        weaponTransform.get()->xpos = entityTransform.get()->xpos + spriteOffset;
        weaponTransform.get()->ypos = entityTransform.get()->ypos; 
    
    }
    else if (entityDirection == WEST) 
    {
        weaponTransform.get()->xpos = entityTransform.get()->xpos - spriteOffset;
        weaponTransform.get()->ypos = entityTransform.get()->ypos; 
    }

    // Update body transform
    ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
    entityBody.get()->body->SetTransform(b2Vec2(weaponTransform.get()->xpos, weaponTransform.get()->ypos), 0.0f);

    // Disable entity movement
    ComponentHandle<Script> entityScript = userEntity.component<Script>(); 
    if (isPlayer)
            reinterpret_cast<PlayerScript*>(entityScript.get()->script)->SetCanMove(false);
        else
            reinterpret_cast<EnemyScript*>(entityScript.get()->script)->SetCanMove(false);

    // Activate entity
    ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
    activeComp.get()->isActive = true;
    entityBody.get()->body->SetEnabled(true);
}

void SwordScript::CreateEntity() 
{
    // Set up entity components
    GetEntity()->assign<TextureComp>("src/Assets/textures/SwordSlash.png", "SwordSlash.png");
    GetEntity()->assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    
    // Transform
    ComponentHandle<Transform> entityTransform = userEntity.component<Transform>(); 
    GetEntity()->assign<Transform>(entityTransform.get()->xpos, entityTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2); 

    GetEntity()->remove<SpriteVertices>();
    std::vector<float> spriteVertices = {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f,
        };
    GetEntity()->assign<SpriteVertices>(spriteVertices);

    // RigidBody
    uint16 categoryBit;
    uint16 maskBit;
    if (isPlayer)
    {
        categoryBit = PhysicsManager::Instance().PLAYERWEAPON;
        maskBit = PhysicsManager::Instance().BOUNDARY | PhysicsManager::Instance().ENEMY;
    }
    else
    {
        categoryBit = PhysicsManager::Instance().ENEMYWEAPON;
        maskBit = PhysicsManager::Instance().BOUNDARY | PhysicsManager::Instance().PLAYER;
    }
    GetEntity()->assign<RigidBody>(entityTransform.get()->xpos, entityTransform.get()->ypos + spriteOffset, 5.0f, 5.0f, 1.0, 0.5f, 1, categoryBit, maskBit);

    ComponentHandle<RigidBody> physicsComp = GetEntity()->component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::Instance().GetWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->CreateFixture();
    physicsComp.get()->SetUserData(GetEntity());

    // Active   
    ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
    activeComp.get()->isActive = false;
    physicsComp.get()->body->SetEnabled(false);
}

// Collision detection
void SwordScript::BeginContact(Entity* entityA, Entity* entityB) { }

void SwordScript::EndContact(Entity* entityA, Entity* entityB) { }
