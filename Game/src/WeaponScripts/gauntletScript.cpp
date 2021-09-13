#include "GauntletScript.h"

#include "../Physics/PhysicsManager.h"
#include "../Components.h"
#include "../Scripts.h"
#include "../ECS.h"

// Blocks attack
// Cooldown between blocks
// Small knockback after block

GauntletScript::GauntletScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity, spriteHeight, spriteWidth)
{
    damage = 15;
    spriteOffset = 8.5f;
    attackRate = 0;
    isActive = false;
}

void GauntletScript::Start() 
{
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        entityName = e.component<Name>();
        if (entityName.get()->name == "Player")
            player = e;
    }

    // Set up entity components
    GetEntity()->assign<TextureComp>("src/Assets/textures/GauntletAttack.png", "GauntletAttack.png");
    GetEntity()->assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    
    // Transform
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    GetEntity()->assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2); 

    GetEntity()->remove<SpriteVertices>();
    std::vector<float> spriteVertices =  {
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
    GetEntity()->assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 2.0f, 2.0f, 1.0, 0.5f, 1, categoryBit, maskBit);

    ComponentHandle<RigidBody> physicsComp = GetEntity()->component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::Instance().GetWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->CreateFixture();
    physicsComp.get()->SetUserData(GetEntity());

    // Active
    ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
    activeComp.get()->isActive = false;
    physicsComp.get()->body->SetEnabled(false);
}

void GauntletScript::Update(TimeDelta dt) 
{
    ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
    if (entityBody.get()->body->IsEnabled()) 
    {
        attackRate -= dt;
        if (attackRate > 0)
            return;

        // Disable collision
        entityBody.get()->body->SetEnabled(false);

        // Disable entity
        ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
        activeComp.get()->isActive = false;

        // Enable player movement
        ComponentHandle<Script> playerScript = player.component<Script>(); 
        reinterpret_cast<PlayerScript*>(playerScript.get()->script)->SetCanPlayerMove(true);
    }
}

void GauntletScript::UseWeapon() 
{
    if (isActive)
        return;

    isActive = true;
    attackRate = 0.5f;

    // Update sword's transform
    ComponentHandle<Transform> entityTransform = GetEntity()->component<Transform>();
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 

    int playerDirection = GetPlayerDirection();
    // Up or no direction found
    if (playerDirection <= 1) 
    {
        entityTransform.get()->xpos = playerTransform.get()->xpos;
        entityTransform.get()->ypos = playerTransform.get()->ypos + spriteOffset;
    
    } // Down 
    else if (playerDirection == 2) 
    {
        entityTransform.get()->xpos = playerTransform.get()->xpos;
        entityTransform.get()->ypos = playerTransform.get()->ypos - spriteOffset; 

    } // Right 
    else if (playerDirection == 3) 
    {
        entityTransform.get()->xpos = playerTransform.get()->xpos + spriteOffset;
        entityTransform.get()->ypos = playerTransform.get()->ypos; 
    
    } // Left
    else if (playerDirection == 4) 
    {
        entityTransform.get()->xpos = playerTransform.get()->xpos - spriteOffset;
        entityTransform.get()->ypos = playerTransform.get()->ypos; 
    }

    // Update body transform
    ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
    entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0.0f);

    // Disable player movement
    ComponentHandle<Script> playerScript = player.component<Script>(); 
    reinterpret_cast<PlayerScript*>(playerScript.get()->script)->SetCanPlayerMove(false);

    // Activate entity
    ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
    activeComp.get()->isActive = true;
    entityBody.get()->body->SetEnabled(true);
}

// Up = 1, Down = 2, Right = 3, Left = 4
int GauntletScript::GetPlayerDirection() 
{
    ComponentHandle<TextureComp> playerTexture = player.component<TextureComp>();
    if (playerTexture.get()->filename == "PlayerUp.png") 
        return 1;

    if (playerTexture.get()->filename == "PlayerDown.png") 
        return 2;

    if (playerTexture.get()->filename == "PlayerRight.png")
        return 3;

    if (playerTexture.get()->filename == "PlayerLeft.png")
        return 4;

    // No direction found
    return 0;
}

// Collision detection
void GauntletScript::BeginContact(Entity* entityA, Entity* entityB) { }

void GauntletScript::EndContact(Entity* entityA, Entity* entityB) { }
