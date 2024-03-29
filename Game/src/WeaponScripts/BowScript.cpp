#include "BowScript.h"

#include "../Physics/PhysicsManager.h"
#include "../Components.h"
#include "../Scripts.h"
#include "../ECS.h"
#include "../CScript.h"

BowScript::BowScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity, spriteHeight, spriteWidth)
{
    damage = 5;
    spriteOffset = 10.0f;
    projectileVelocity = 250.0f;
    projectileLifespan = 3.0f;

    // NOTE: Max number of projectiles was set to one in order to simplify
    //       the state being used to train the agent. If higher, the state would 
    //       need to take note of each indivdual projectile every frame even if 
    //       one didn't exist. Explained further in the final report.
    maxNumberOfProjectiles = 1; 

    fireRate = 0.0f;
    isActive = false;
    isPlayer = false;
    canDamageShield = false;
}

void BowScript::Start() 
{
    // Get name of entity for later
    ComponentHandle<Name> weapon = GetEntity()->component<Name>();
    std::string weaponName = weapon->name;

    // Get reference to entity
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::Instance().entities.entities_with_components(entityName)) 
    {
        entityName = e.component<Name>();
        if (entityName.get()->name == "Player" && weaponName.find("Player") != std::string::npos)
        {
            userEntity = e;
            isPlayer = true;
        }

        if (entityName.get()->name == "Enemy" && weaponName.find("Enemy") != std::string::npos)
            userEntity = e;
    }
}

void BowScript::Update(TimeDelta dt) 
{
    // Delete any projectiles flagged for deletion
    for (std::string entityName : projectilesFlaggedForDeletion)
    {
        for (int i = 0; i < projectilesTimeElapsed.size(); i++) 
        {        
            ComponentHandle<Name> nameComp = projectiles.at(i).first.component<Name>();
            if (nameComp.get()->name == entityName) 
            {
                Entity e = projectiles.at(i).first;
                projectiles.erase(projectiles.begin() + i);
                projectilesTimeElapsed.erase(projectilesTimeElapsed.begin() + i);

                ComponentHandle<RigidBody> physicsComp = e.component<RigidBody>();
                PhysicsManager::Instance().GetWorld()->DestroyBody(physicsComp.get()->body);

                e.destroy();
                break;
            }
        }
    }
    projectilesFlaggedForDeletion.clear();

    // Update cooldown
    float cooldown = fireRate - dt;
    if (cooldown < 0)
        fireRate = 0;
    else
        fireRate = cooldown;

    // Update position of arrows
    for (std::pair<Entity, int> projectile : projectiles)
    { 
        float desiredVelX = 0;
        float desiredVelY = 0;

        switch(projectile.second)
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
        
        ComponentHandle<RigidBody> entityBody = projectile.first.component<RigidBody>();
        b2Vec2 projectileVel = entityBody.get()->body->GetLinearVelocity();
        float velChangeX = desiredVelX - projectileVel.x;
        float velChangeY = desiredVelY - projectileVel.y;
        float impulseX = entityBody.get()->body->GetMass() * velChangeX;
        float impulseY = entityBody.get()->body->GetMass() * velChangeY;
        entityBody.get()->body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), entityBody.get()->body->GetWorldCenter(), true);

        ComponentHandle<Transform> projectileTransform = projectile.first.component<Transform>();
        projectileTransform.get()->xpos = entityBody.get()->body->GetTransform().p.x;
        projectileTransform.get()->ypos = entityBody.get()->body->GetTransform().p.y;
    }

    // Update time elapsed for each projectile and delete if they exceed the lifespan
    bool deletedProjectile = false;
    for (int i = 0; i < projectilesTimeElapsed.size(); i++) 
    {
        projectilesTimeElapsed.at(i) += dt;
        if (projectilesTimeElapsed.at(i) >= projectileLifespan) 
        {
            deletedProjectile = true;
            
            Entity e = projectiles.at(0).first;
            projectiles.erase(projectiles.begin());

            ComponentHandle<RigidBody> physicsComp = e.component<RigidBody>();
            PhysicsManager::Instance().GetWorld()->DestroyBody(physicsComp.get()->body);

            e.destroy();
            break;
        }
    }

    // Remove the timeElapsed if any projectiles were deleted
    if (deletedProjectile)
        projectilesTimeElapsed.erase(projectilesTimeElapsed.begin());

    // Player movement
    if (isActive)
    {
         // Slow player movement
        ComponentHandle<Script> userScript = userEntity.component<Script>();
        if (isPlayer) 
            reinterpret_cast<PlayerScript*>(userScript.get()->script)->SetIsMovementReduced(true);
        else
            reinterpret_cast<EnemyScript*>(userScript.get()->script)->SetIsMovementReduced(true);
    }
    else
    {
         // Enable player movement
        ComponentHandle<Script> userScript = userEntity.component<Script>();
        if (isPlayer) 
            reinterpret_cast<PlayerScript*>(userScript.get()->script)->SetIsMovementReduced(false);
        else
            reinterpret_cast<EnemyScript*>(userScript.get()->script)->SetIsMovementReduced(false);
    }
}

void BowScript::UseWeapon() 
{
    isActive = true;
    if (fireRate > 0 || projectiles.size() >= maxNumberOfProjectiles)
        return;

    fireRate = 1.5f;
    SpawnArrow();
}

void BowScript::SpawnArrow() 
{
    // Set up entity components
    Entity e = ECS::Instance().entities.create();

    e.assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    e.assign<Active>(true);

    std::vector<float> spriteVertices =  {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f,
        };
    e.assign<SpriteVertices>(spriteVertices);

    ComponentHandle<Name> weapon = GetEntity()->component<Name>();
    std::string name = weapon->name + "_Arrow" + std::to_string(projectiles.size());
    e.assign<Name>(name);
    e.assign<Script>(cscript);

    // Rigidbody bits
    uint16 categoryBit;
    uint16 maskBit;
    if (isPlayer)
    {
        categoryBit = PhysicsManager::Instance().PLAYERWEAPON;
        maskBit = PhysicsManager::Instance().BOUNDARY | PhysicsManager::Instance().ENEMY | PhysicsManager::Instance().ENEMYWEAPON;
    }
    else
    {
        categoryBit = PhysicsManager::Instance().ENEMYWEAPON;
        maskBit = PhysicsManager::Instance().BOUNDARY | PhysicsManager::Instance().PLAYER | PhysicsManager::Instance().PLAYERWEAPON;
    }

    // Transform
    ComponentHandle<Transform> userTransform = userEntity.component<Transform>(); 

    Direction userDirection = GetDirection(&userEntity);
    switch(userDirection) 
    {
        case NORTH:
            e.assign<TextureComp>("src/Assets/textures/ArrowUp.png", "ArrowUp.png");
            e.assign<Transform>(userTransform.get()->xpos, userTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2);
            e.assign<RigidBody>(userTransform.get()->xpos, userTransform.get()->ypos + spriteOffset, 2.0f, 4.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        case SOUTH:
            e.assign<TextureComp>("src/Assets/textures/ArrowDown.png", "ArrowDown.png");
            e.assign<Transform>(userTransform.get()->xpos, userTransform.get()->ypos - spriteOffset, 0.0f, 0, 0, 0, 1, 2);
            e.assign<RigidBody>(userTransform.get()->xpos, userTransform.get()->ypos - spriteOffset, 2.0f, 4.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        case EAST:
            e.assign<TextureComp>("src/Assets/textures/ArrowRight.png", "ArrowRight.png");
            e.assign<Transform>(userTransform.get()->xpos + spriteOffset, userTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 
            e.assign<RigidBody>(userTransform.get()->xpos + spriteOffset, userTransform.get()->ypos, 4.0f, 2.0f, 1.0, 0.5f, 1, categoryBit, maskBit);
            break;
        case WEST:
            e.assign<TextureComp>("src/Assets/textures/ArrowLeft.png", "ArrowLeft.png");
            e.assign<Transform>(userTransform.get()->xpos - spriteOffset, userTransform.get()->ypos, 0.0f, 0, 0, 0.0f, 1, 2);
            e.assign<RigidBody>(userTransform.get()->xpos - spriteOffset, userTransform.get()->ypos, 4.0f, 2.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
            break;
        default:
            return;
    }

    ComponentHandle<RigidBody> physicsComp = e.component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::Instance().GetWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->CreateFixture();
    physicsComp.get()->SetUserData(&e);
    physicsComp.get()->SetSensor(true);

    // Add entity to list
    projectiles.push_back(std::make_pair(ECS::Instance().entities.get(e.id()), userDirection));
    projectilesTimeElapsed.push_back(0.0f);
}

// Collision detection
void BowScript::BeginContact(Entity* entityA, Entity* entityB) 
{
    ComponentHandle<Name> nameComp = entityA->component<Name>();
    if (nameComp.get()->name.find("_Arrow") != std::string::npos)
    {
        projectilesFlaggedForDeletion.push_back(nameComp.get()->name);
    }
}

void BowScript::EndContact(Entity* entityA, Entity* entityB) { }
