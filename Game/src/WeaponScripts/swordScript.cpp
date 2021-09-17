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

    // Member variables
    spriteOffset = 10.5f;
    attackRate = 0.3f;
    attackCooldown = 0.0f;
}

void SwordScript::Start() 
{
    // Get reference to player
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        entityName = e.component<Name>();
        if (entityName.get()->name == "Player")
            player = e;
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

        // Enable player movement
        ComponentHandle<Script> playerScript = player.component<Script>(); 
        reinterpret_cast<PlayerScript*>(playerScript.get()->script)->SetCanMove(true);
    }
}

void SwordScript::UseWeapon() 
{
    if (isActive)
        return;

    isActive = true;
    attackCooldown = attackRate;

    // Update sword's transform
    ComponentHandle<Transform> entityTransform = GetEntity()->component<Transform>();
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 

    // Determine the direction to use the weapon
    Direction playerDirection = GetDirection(&player);
    if (playerDirection == NORTH) 
    {
        entityTransform.get()->xpos = playerTransform.get()->xpos;
        entityTransform.get()->ypos = playerTransform.get()->ypos + spriteOffset; 
    
    }
    else if (playerDirection == SOUTH) 
    {
        entityTransform.get()->xpos = playerTransform.get()->xpos;
        entityTransform.get()->ypos = playerTransform.get()->ypos - spriteOffset; 
    
    }
    else if (playerDirection == EAST) 
    {
        entityTransform.get()->xpos = playerTransform.get()->xpos + spriteOffset;
        entityTransform.get()->ypos = playerTransform.get()->ypos; 
    
    }
    else if (playerDirection == WEST) 
    {
        entityTransform.get()->xpos = playerTransform.get()->xpos - spriteOffset;
        entityTransform.get()->ypos = playerTransform.get()->ypos; 
    }

    // Update body transform
    ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
    entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0.0f);

    // Disable player movement
    ComponentHandle<Script> playerScript = player.component<Script>(); 
    reinterpret_cast<PlayerScript*>(playerScript.get()->script)->SetCanMove(false);

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
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    GetEntity()->assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2); 

    GetEntity()->remove<SpriteVertices>();
    std::vector<float> spriteVertices = {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f,
        };
    GetEntity()->assign<SpriteVertices>(spriteVertices);

    // RigidBody
    // TODO: Factor for enemy using weapon
    uint16 categoryBit = PhysicsManager::Instance().PLAYERWEAPON;
    uint16 maskBit = PhysicsManager::Instance().BOUNDARY | PhysicsManager::Instance().ENEMY;
    GetEntity()->assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 5.0f, 5.0f, 1.0, 0.5f, 1, categoryBit, maskBit);

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
