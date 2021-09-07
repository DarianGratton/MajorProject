#include "ShieldScript.h"

#include "../Physics/PhysicsManager.h"
#include "../Components.h"
#include "../Scripts.h"
#include "../ECS.h"

ShieldScript::ShieldScript(entityx::Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity), spriteHeight(spriteHeight), spriteWidth(spriteWidth) 
{
    damage = 0;
    shieldCooldown = 0;
    isActive = false;
    hitVelocity = b2Vec2_zero;
}

void ShieldScript::Start() 
{
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        entityName = e.component<Name>();
        
        if (entityName.get()->name == "Player")
            player = e;
    }

    // Set up entity components
    GetEntity()->assign<TextureComp>("src/Assets/textures/Shield.png");
    GetEntity()->assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    
    // Transform
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    GetEntity()->assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 

    GetEntity()->remove<SpriteVertices>();
    std::vector<float> spriteVertices =  {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f
        };
    GetEntity()->assign<SpriteVertices>(spriteVertices);

    // RigidBody
    // TODO: Factor for enemy using weapon
    uint16 categoryBit = PhysicsManager::Instance().PLAYERWEAPON;
    uint16 maskBit = PhysicsManager::Instance().BOUNDARY | PhysicsManager::Instance().ENEMY | PhysicsManager::Instance().ENEMYWEAPON;
    GetEntity()->assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos, 5.0f, 5.0f, 1.0, 0.5f, 1, categoryBit, maskBit);

    ComponentHandle<RigidBody> physicsComp = GetEntity()->component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::Instance().GetWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->CreateFixture();
    physicsComp.get()->SetUserData(GetEntity());

    // Active
    ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
    activeComp.get()->isActive = false;
    physicsComp.get()->body->SetEnabled(false);
}

void ShieldScript::Update(TimeDelta dt) 
{
    // Update cooldown
    float cooldown = shieldCooldown - dt;
    if (cooldown <= 0) 
    {
        shieldCooldown = 0;
    } 
    else 
    {
        shieldCooldown = cooldown;
        return;
    }

    ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
    if (!isActive && entityBody.get()->body->IsEnabled()) 
    {
        // Disable collision
        entityBody.get()->body->SetEnabled(false);

        // Disable entity
        ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
        activeComp.get()->isActive = false;

        // Enable player movement
        ComponentHandle<Script> playerScript = player.component<Script>(); 
        reinterpret_cast<PlayerScript*>(playerScript.get()->script)->SetCanPlayerMove(true);

        // Enable player body
        ComponentHandle<RigidBody> playerBody = player.component<RigidBody>(); 
        playerBody.get()->body->SetEnabled(true);
        playerBody.get()->body->SetLinearVelocity(hitVelocity);
        hitVelocity = b2Vec2_zero;
    }
}

void ShieldScript::UseWeapon() 
{
    if (isActive)
        return;

    isActive = true; 

    // Update shield's transform
    ComponentHandle<Transform> entityTransform = GetEntity()->component<Transform>();
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    entityTransform.get()->xpos = playerTransform.get()->xpos;
    entityTransform.get()->ypos = playerTransform.get()->ypos;

    // Update body transform
    ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
    entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0.0f);

    // Disable player movement
    ComponentHandle<Script> playerScript = player.component<Script>(); 
    reinterpret_cast<PlayerScript*>(playerScript.get()->script)->SetCanPlayerMove(false);

    // Disable player body
    ComponentHandle<RigidBody> playerBody = player.component<RigidBody>(); 
    playerBody.get()->body->SetEnabled(false);

    // Activate entity
    ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
    activeComp.get()->isActive = true;
    entityBody.get()->body->SetEnabled(true);
}

// Collision detection
void ShieldScript::BeginContact(Entity* entityA, Entity* entityB) 
{
    ComponentHandle<Name> nameComp = entityB->component<Name>();
    if (!(nameComp.get()->name.find("Enemy") != std::string::npos)) 
    {
        isActive = false;

        ComponentHandle<RigidBody> shieldBody = entityA->component<RigidBody>();
        hitVelocity = shieldBody.get()->body->GetLinearVelocity();

        shieldCooldown = 3.0f;
    }
}

void ShieldScript::EndContact(Entity* entityA, Entity* entityB) {

}
