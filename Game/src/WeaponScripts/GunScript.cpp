#include "GunScript.h"

#include "../Physics/PhysicsManager.h"
#include "../Components.h"
#include "../Scripts.h"
#include "../ECS.h"
#include "../CScript.h"

GunScript::GunScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity, spriteHeight, spriteWidth)
{
    // Parent member variables
    damage = 20;
    isActive = false;
    canDamageShield = true;

    // Member variables
    spriteOffset = 10.0f;
    projectileVelocity = 115.0f;
    cooldownTimer = 0.0f;
    timeElapsed = 0.0f;
    directionThrown = NONE;
    flagForDeletion = false;
    isPlayer = false;
}

void GunScript::Start() 
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
}

void GunScript::Update(TimeDelta dt) 
{
    // Update cooldown
    float cooldown = cooldownTimer - dt;
    if (cooldown <= 0) 
        cooldownTimer = 0;
    else 
        cooldownTimer = cooldown;

    timeElapsed += dt;

    // Delete bullet
    if (bulletEntity.valid() && flagForDeletion) 
    {
        ComponentHandle<RigidBody> physicsComp = bulletEntity.component<RigidBody>();
        PhysicsManager::Instance().GetWorld()->DestroyBody(physicsComp.get()->body);

        bulletEntity.destroy();
        directionThrown = NONE;
        timeElapsed = 0.0f;
        flagForDeletion = false;
    }

    // Update position of bullet
    if (bulletEntity.valid()) 
    { 
        float desiredVelX = 0;
        float desiredVelY = 0;

        ComponentHandle<Transform> entityTransform = bulletEntity.component<Transform>();
        switch(directionThrown)
        {
            case NORTH:
                desiredVelY = projectileVelocity;
                break;
            case SOUTH:
                desiredVelY = -projectileVelocity;
                break;
            case EAST:
                desiredVelX = projectileVelocity;
                break;
            case WEST:
                desiredVelX = -projectileVelocity;
                break;
            default:
                break;
        }
        
        ComponentHandle<RigidBody> entityBody = bulletEntity.component<RigidBody>();
        b2Vec2 grenadeVelocity = entityBody.get()->body->GetLinearVelocity();
    
        float velChangeX = desiredVelX - grenadeVelocity.x;
        float velChangeY = desiredVelY - grenadeVelocity.y;
        float impulseX = entityBody.get()->body->GetMass() * velChangeX;
        float impulseY = entityBody.get()->body->GetMass() * velChangeY;
        entityBody.get()->body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), entityBody.get()->body->GetWorldCenter(), true);
    
        // Update bullet entity position
        ComponentHandle<Transform> transform = bulletEntity.component<Transform>();
        transform.get()->xpos = entityBody.get()->body->GetPosition().x;
        transform.get()->ypos = entityBody.get()->body->GetPosition().y;
    }
}

void GunScript::UseWeapon() 
{
    if (cooldownTimer > 0 || isActive)
        return;
        
    cooldownTimer = 1.0f;
    timeElapsed = 0.0f;
    SpawnBullet();
}

void GunScript::SpawnBullet() 
{
    // Set up entity components
    bulletEntity = ECS::Instance().entities.create();

    // TODO: Change texture
    bulletEntity.assign<TextureComp>("src/Assets/textures/Bullet.png", "Bullet.png");
    bulletEntity.assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    bulletEntity.assign<Active>(true);

    std::vector<float> spriteVertices =  {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f,
        };
    bulletEntity.assign<SpriteVertices>(spriteVertices);

    bulletEntity.assign<Name>("WeaponBullet");

    // Rigidbody bits
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

    // Transform
    Direction userDirection = GetDirection(&userEntity);
    ComponentHandle<Transform> userTransform = userEntity.component<Transform>(); 
    switch(userDirection)
    {
        case NORTH:
            bulletEntity.assign<Transform>(userTransform.get()->xpos, userTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2);
            bulletEntity.assign<RigidBody>(userTransform.get()->xpos, userTransform.get()->ypos + spriteOffset, 4.0f, 4.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        case SOUTH:
            bulletEntity.assign<Transform>(userTransform.get()->xpos, userTransform.get()->ypos - spriteOffset, 0.0f, 0, 0, 0, 1, 2);
            bulletEntity.assign<RigidBody>(userTransform.get()->xpos, userTransform.get()->ypos - spriteOffset, 4.0f, 4.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        case EAST:
            bulletEntity.assign<Transform>(userTransform.get()->xpos + spriteOffset, userTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 
            bulletEntity.assign<RigidBody>(userTransform.get()->xpos + spriteOffset, userTransform.get()->ypos, 4.0f, 4.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        case WEST:
            bulletEntity.assign<Transform>(userTransform.get()->xpos - spriteOffset, userTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2);
            bulletEntity.assign<RigidBody>(userTransform.get()->xpos - spriteOffset, userTransform.get()->ypos, 4.0f, 4.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        default:
            break;
    }
    
    // Assign script
    bulletEntity.assign<Script>(cscript);

    ComponentHandle<RigidBody> physicsComp = bulletEntity.component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::Instance().GetWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->CreateFixture();
    physicsComp.get()->SetUserData(&bulletEntity);

    // Update variables
    directionThrown = userDirection;
}

// Collision detection
void GunScript::BeginContact(Entity* entityA, Entity* entityB) 
{ 
    ComponentHandle<Name> entityNameA = entityA->component<Name>();
    ComponentHandle<Name> entityNameB = entityB->component<Name>();
    if (entityNameB.get()->name.find("Enemy") != string::npos ||
        entityNameB.get()->name.find("Wall") != string::npos) 
    {
        flagForDeletion = true;
        timeElapsed = 0.0f;
        cooldownTimer = 1.0f;
    }
}

void GunScript::EndContact(Entity* entityA, Entity* entityB) { }
