#include "ShieldScript.h"

#include "../Physics/PhysicsManager.h"
#include "../Components.h"
#include "../Scripts.h"
#include "../ECS.h"

ShieldScript::ShieldScript(entityx::Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity, spriteHeight, spriteWidth)
{
    // Parent class variables
    damage = 0;
    isActive = false;
    canDamageShield = false;

    // Member variables
    shieldMaxCooldown = 3.0f;
    shieldCurrCooldown = 0.0f;
    hitVelocity = b2Vec2_zero;
    shieldMaxHealth = 10;
    shieldCurrHealth = shieldMaxHealth;
    isPlayer = false;
}

void ShieldScript::Start() 
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

    CreateEntity();
}

void ShieldScript::Update(TimeDelta dt) 
{
    // Update cooldown
    float cooldown = shieldCurrCooldown - dt;
    if (cooldown <= 0) 
    {
        shieldCurrCooldown = 0;
    } 
    else 
    {
        shieldCurrCooldown = cooldown;
        return;
    }

    if (isActive)
    {
        // Move player based on entity body
        ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
        ComponentHandle<Script> userScript = userEntity.component<Script>();
        if (isPlayer) 
            reinterpret_cast<PlayerScript*>(userScript.get()->script)->MoveCharacter(entityBody.get()->body);
        else 
            reinterpret_cast<EnemyScript*>(userScript.get()->script)->MoveCharacter(entityBody.get()->body);

        // Update entity transform
        ComponentHandle<Transform> entityTransform = GetEntity()->component<Transform>();
        ComponentHandle<Transform> userTransform = userEntity.component<Transform>(); 
        entityTransform.get()->xpos = userTransform.get()->xpos;
        entityTransform.get()->ypos = userTransform.get()->ypos;

        // Update user body transform (used because of movement logic)
        ComponentHandle<RigidBody> userBody = userEntity.component<RigidBody>();
        userBody.get()->body->SetTransform(b2Vec2(entityBody.get()->body->GetTransform().p.x, entityBody.get()->body->GetTransform().p.y), 0.0f);
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
        ComponentHandle<Script> userScript = userEntity.component<Script>();
        if (isPlayer) 
            reinterpret_cast<PlayerScript*>(userScript.get()->script)->SetIsMovementReduced(false);
        else
            reinterpret_cast<EnemyScript*>(userScript.get()->script)->SetIsMovementReduced(false);

        // Enable user body
        ComponentHandle<Transform> userTransform = userEntity.component<Transform>();
        ComponentHandle<RigidBody> userBody = userEntity.component<RigidBody>(); 
        userBody.get()->body->SetTransform(b2Vec2(userTransform.get()->xpos, userTransform.get()->ypos), 0.0f);
        userBody.get()->body->SetEnabled(true);
        // playerBody.get()->body->SetLinearVelocity(hitVelocity);
        // hitVelocity = b2Vec2_zero;
    }
}

void ShieldScript::UseWeapon() 
{
    if (isActive)
        return;

    isActive = true; 

    // Update shield's transform
    ComponentHandle<Transform> entityTransform = GetEntity()->component<Transform>();
    ComponentHandle<Transform> userTransform = userEntity.component<Transform>(); 
    entityTransform.get()->xpos = userTransform.get()->xpos;
    entityTransform.get()->ypos = userTransform.get()->ypos;

    // Update body transform
    ComponentHandle<RigidBody> entityBody = GetEntity()->component<RigidBody>();
    entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0.0f);

    // Reduce player movement
    // TODO
    ComponentHandle<Script> userScript = userEntity.component<Script>(); 
    if (isPlayer)
        reinterpret_cast<PlayerScript*>(userScript.get()->script)->SetIsMovementReduced(true);
    else
        reinterpret_cast<EnemyScript*>(userScript.get()->script)->SetIsMovementReduced(true);

    // Disable player body
    ComponentHandle<RigidBody> userBody = userEntity.component<RigidBody>(); 
    userBody.get()->body->SetEnabled(false);

    // Activate entity
    ComponentHandle<Active> activeComp = GetEntity()->component<Active>();
    activeComp.get()->isActive = true;
    entityBody.get()->body->SetEnabled(true);
}

void ShieldScript::CreateEntity()
{
    // Set up entity components
    GetEntity()->assign<TextureComp>("src/Assets/textures/Shield.png");
    GetEntity()->assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    
    // Transform
    ComponentHandle<Transform> userTransform = userEntity.component<Transform>(); 
    GetEntity()->assign<Transform>(userTransform.get()->xpos, userTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 

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
    GetEntity()->assign<RigidBody>(userTransform.get()->xpos, userTransform.get()->ypos, 5.0f, 5.0f, 1.0, 0.5f, 1, categoryBit, maskBit);

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
void ShieldScript::BeginContact(Entity* entityA, Entity* entityB) 
{
    ComponentHandle<Name> nameComp = entityB->component<Name>();
    if (nameComp.get()->name.find("Weapon") != std::string::npos) 
    {
        ComponentHandle<RigidBody> shieldBody = entityA->component<RigidBody>();
        hitVelocity = shieldBody.get()->body->GetLinearVelocity();

        ComponentHandle<Script> weaponScript = entityB->component<Script>();
        
        if (!reinterpret_cast<WeaponScript*>(weaponScript.get()->script)->CanDamageShield())
            return; 

        int damage = reinterpret_cast<WeaponScript*>(weaponScript.get()->script)->GetDamage();
        shieldCurrHealth -= damage;

        if (shieldCurrHealth < 0)
        {
            shieldCurrCooldown = shieldMaxCooldown;
            shieldCurrHealth = shieldMaxHealth;
            isActive = false;       
        }
    }
}

void ShieldScript::EndContact(Entity* entityA, Entity* entityB) {

}
