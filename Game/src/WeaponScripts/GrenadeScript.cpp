#include "GrenadeScript.h"

#include "../Physics/PhysicsManager.h"
#include "../Components.h"
#include "../Scripts.h"
#include "../ECS.h"
#include "../CScript.h"

GrenadeScript::GrenadeScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity, spriteHeight, spriteWidth)
{
    // Parent member variables
    damage = 30;
    isActive = false;
    canDamageShield = true;

    // Member variables
    spriteOffset = 10.0f;
    projectileVelocity = 75.0f;
    projectileLifespan = 5.0f;
    explosionLifespan = 3.0f;
    cooldownTimer = 0.0f;
    timeElapsed = 0.0f;
    directionThrown = NONE;
}

void GrenadeScript::Start() 
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

void GrenadeScript::Update(TimeDelta dt) 
{
    // Update cooldown
    float cooldown = cooldownTimer - dt;
    if (cooldown <= 0) 
        cooldownTimer = 0;
    else 
        cooldownTimer = cooldown;

    timeElapsed += dt;

    // Explode grenade
    if (grenadeEntity.valid() && timeElapsed >= projectileLifespan) 
    {
        SpawnExplosion();

        ComponentHandle<RigidBody> physicsComp = grenadeEntity.component<RigidBody>();
        PhysicsManager::Instance().GetWorld()->DestroyBody(physicsComp.get()->body);

        grenadeEntity.destroy();
        directionThrown = NONE;
        timeElapsed = 0.0f;
    }

    // Destroy explosion
    if (explosionEntity.valid() && timeElapsed >= explosionLifespan) 
    {
        ComponentHandle<RigidBody> physicsComp = explosionEntity.component<RigidBody>();
        PhysicsManager::Instance().GetWorld()->DestroyBody(physicsComp.get()->body);

        explosionEntity.destroy();
    }

    // Update position of grenade
    if (grenadeEntity.valid()) 
    { 
        float desiredVelX = 0;
        float desiredVelY = 0;

        ComponentHandle<Transform> entityTransform = grenadeEntity.component<Transform>();
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
        
        ComponentHandle<RigidBody> entityBody = grenadeEntity.component<RigidBody>();
        b2Vec2 grenadeVelocity = entityBody.get()->body->GetLinearVelocity();
    
        float velChangeX = desiredVelX - grenadeVelocity.x;
        float velChangeY = desiredVelY - grenadeVelocity.y;
        float impulseX = entityBody.get()->body->GetMass() * velChangeX;
        float impulseY = entityBody.get()->body->GetMass() * velChangeY;
        entityBody.get()->body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), entityBody.get()->body->GetWorldCenter(), true);
    
        // Update player position
        ComponentHandle<Transform> transform = grenadeEntity.component<Transform>();
        transform.get()->xpos = entityBody.get()->body->GetPosition().x;
        transform.get()->ypos = entityBody.get()->body->GetPosition().y;
    }
}

void GrenadeScript::UseWeapon() 
{
    if (cooldownTimer > 0 || isActive)
        return;
        
    cooldownTimer = projectileLifespan + explosionLifespan + 3.0f;
    timeElapsed = 0.0f;
    SpawnGrenade();
}

void GrenadeScript::SpawnGrenade() 
{
    // Set up entity components
    grenadeEntity = ECS::Instance().entities.create();

    grenadeEntity.assign<TextureComp>("src/Assets/textures/Grenade.png", "Grenade.png");
    grenadeEntity.assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    grenadeEntity.assign<Active>(true);

    std::vector<float> spriteVertices =  {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f,
        };
    grenadeEntity.assign<SpriteVertices>(spriteVertices);

    grenadeEntity.assign<Name>("Grenade");
    
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
            grenadeEntity.assign<Transform>(userTransform.get()->xpos, userTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2);
            grenadeEntity.assign<RigidBody>(userTransform.get()->xpos, userTransform.get()->ypos + spriteOffset, 1.0f, 1.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        case SOUTH:
            grenadeEntity.assign<Transform>(userTransform.get()->xpos, userTransform.get()->ypos - spriteOffset, 0.0f, 0, 0, 0, 1, 2);
            grenadeEntity.assign<RigidBody>(userTransform.get()->xpos, userTransform.get()->ypos - spriteOffset, 1.0f, 1.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        case EAST:
            grenadeEntity.assign<Transform>(userTransform.get()->xpos + spriteOffset, userTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 
            grenadeEntity.assign<RigidBody>(userTransform.get()->xpos + spriteOffset, userTransform.get()->ypos, 1.0f, 1.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        case WEST:
            grenadeEntity.assign<Transform>(userTransform.get()->xpos - spriteOffset, userTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2);
            grenadeEntity.assign<RigidBody>(userTransform.get()->xpos - spriteOffset, userTransform.get()->ypos, 1.0f, 1.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        default:
            break;
    }
    
    // Assign script
    grenadeEntity.assign<Script>(cscript);

    ComponentHandle<RigidBody> physicsComp = grenadeEntity.component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::Instance().GetWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->CreateFixture();
    physicsComp.get()->SetUserData(&grenadeEntity);

    // Update variables
    directionThrown = userDirection;
}

void GrenadeScript::SpawnExplosion() 
{
    // Set up entity components
    explosionEntity = ECS::Instance().entities.create();

    explosionEntity.assign<TextureComp>("src/Assets/textures/Explosion.png", "Explosion.png");
    explosionEntity.assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    explosionEntity.assign<Active>(true);

    float explosionSpriteHeight = spriteHeight * 2;
    float explosionSpriteWidth = spriteWidth * 2;
    std::vector<float> spriteVertices =  {
            -explosionSpriteWidth/2, -explosionSpriteHeight/2, 0.0f, 0.0f,
             explosionSpriteWidth/2, -explosionSpriteHeight/2, 1.0f, 0.0f,
             explosionSpriteWidth/2,  explosionSpriteHeight/2, 1.0f, 1.0f,
            -explosionSpriteWidth/2,  explosionSpriteHeight/2, 0.0f, 1.0f,
        };
    explosionEntity.assign<SpriteVertices>(spriteVertices);

    explosionEntity.assign<Name>("WeaponExplosion");
    
    // Rigidbody bits
    uint16 categoryBit;
    uint16 maskBit;
    if (isPlayer)
    {
        categoryBit = PhysicsManager::Instance().PLAYERWEAPON;
        maskBit = PhysicsManager::Instance().BOUNDARY | PhysicsManager::Instance().ENEMY | PhysicsManager::Instance().PLAYER;
    }
    else
    {
        categoryBit = PhysicsManager::Instance().ENEMYWEAPON;
        maskBit = PhysicsManager::Instance().BOUNDARY | PhysicsManager::Instance().PLAYER | PhysicsManager::Instance().ENEMY;
    }

    // Transform
    ComponentHandle<Transform> grenadeTransform = grenadeEntity.component<Transform>(); 
    explosionEntity.assign<Transform>(grenadeTransform.get()->xpos, grenadeTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2);
    explosionEntity.assign<RigidBody>(grenadeTransform.get()->xpos, grenadeTransform.get()->ypos, 10.0f, 10.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 

    explosionEntity.assign<Script>(cscript);

    ComponentHandle<RigidBody> physicsComp = explosionEntity.component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::Instance().GetWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->CreateFixture();
    physicsComp.get()->SetUserData(&explosionEntity);
}

// Collision detection
void GrenadeScript::BeginContact(Entity* entityA, Entity* entityB) 
{ 
    ComponentHandle<Name> entityNameA = entityA->component<Name>();
    ComponentHandle<Name> entityNameB = entityB->component<Name>();
    if (entityNameA.get()->name.find("Explosion") == string::npos &&
        entityNameB.get()->name.find("Wall") != string::npos) 
    {
        switch(directionThrown)
        {
            case NORTH:
                directionThrown = SOUTH;
                break;
            case SOUTH:
                directionThrown = NORTH;
                break;
            case EAST:
                directionThrown = WEST;
                break;
            case WEST:
                directionThrown = EAST;
                break;
            default:
                LOG_ERROR("GrenadeScript BeginContact(): Error directionThrown not set.");
                break;
        }
    }
}

void GrenadeScript::EndContact(Entity* entityA, Entity* entityB) { }
