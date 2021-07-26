#include "bowScript.h"

#include "../Physics/physicsManager.h"
#include "../components.h"
#include "../scripts.h"
#include "../ECS.h"

// Blocks attack
// Cooldown between blocks
// Small knockback after block

BowScript::BowScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity), spriteHeight(spriteHeight), spriteWidth(spriteWidth) {
    spriteOffset = 10.0f;
    projectileSpeed = 2.0f;
    projectileLifespan = 2.0f;
    fireRate = 0.0f;
    isActive = false;
}

void BowScript::start() {
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "Player")
            player = entity;
    }
}

void BowScript::update(TimeDelta dt) {

    // Update cooldown
    float cooldown = fireRate - dt;
    if (cooldown <= 0) {
        fireRate = 0;
    } else {
        fireRate = cooldown;
    }

    // Update position of arrows
    for (std::pair<Entity, int> projectile : projectiles) { 

        ComponentHandle<Transform> entityTransform = projectile.first.component<Transform>();

        // Up or no direction found
        if (projectile.second <= 1) {
            entityTransform.get()->ypos += projectileSpeed;
        
        // Down
        } else if (projectile.second == 2) {
            entityTransform.get()->ypos -= projectileSpeed;  
        
        // Right
        } else if (projectile.second == 3) {
            entityTransform.get()->xpos += projectileSpeed; 
        
        // Left
        } else if (projectile.second == 4) {
            entityTransform.get()->xpos -= projectileSpeed; 
        }

        ComponentHandle<RigidBody> entityBody = projectile.first.component<RigidBody>();
        entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0);
    }

    // Update time elapsed for each projectile and delete if they exceed the lifespan
    bool deletedProjectile = false;
    for (int i = 0; i < projectilesTimeElapsed.size(); i++) {
        
        projectilesTimeElapsed.at(i) += dt;
        if (projectilesTimeElapsed.at(i) >= projectileLifespan) {
            deletedProjectile = true;
            
            Entity e = projectiles.at(0).first;
            projectiles.erase(projectiles.begin());

            ComponentHandle<RigidBody> physicsComp = e.component<RigidBody>();
            PhysicsManager::instance().getWorld()->DestroyBody(physicsComp.get()->body);

            e.destroy();
            break;
        }

    }

    // Remove the timeElapsed if any projectiles were deleted
    if (deletedProjectile) {
        projectilesTimeElapsed.erase(projectilesTimeElapsed.begin());
    }
}

void BowScript::useWeapon() {
    if (fireRate > 0 || projectiles.size() > 3)
        return;

    fireRate = 0.6f;
    spawnArrow();
}

// Up = 1, Down = 2, Right = 3, Left = 4
int BowScript::getPlayerDirection() {
    ComponentHandle<TextureComp> playerTexture = player.component<TextureComp>();
    if (playerTexture.get()->filename == "PlayerUp.png") {
        return 1;
    }

    if (playerTexture.get()->filename == "PlayerDown.png") {
        return 2;
    }

    if (playerTexture.get()->filename == "PlayerRight.png") {
        return 3;
    }

    if (playerTexture.get()->filename == "PlayerLeft.png") {
        return 4;
    }

    // No direction found
    return 0;
}

void BowScript::spawnArrow() {
    // Set up entity components
    Entity entity = ECS::instance().entities.create();

    int playerDirection = getPlayerDirection();
    if (playerDirection <= 1) {
        entity.assign<TextureComp>("src/Assets/textures/ArrowUp.png", "ArrowUp.png");
    } else if (playerDirection == 2) {
        entity.assign<TextureComp>("src/Assets/textures/ArrowDown.png", "ArrowDown.png");
    } else if (playerDirection == 3) {
        entity.assign<TextureComp>("src/Assets/textures/ArrowRight.png", "ArrowRight.png");
    } else if (playerDirection == 4) {
        entity.assign<TextureComp>("src/Assets/textures/ArrowLeft.png", "ArrowLeft.png");
    }

    entity.assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    entity.assign<Active>(true);

    std::vector<float> spriteVertices =  {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f,
        };
    entity.assign<SpriteVertices>(spriteVertices);

    std::string name = "Arrow" + projectiles.size();
    entity.assign<Name>(name);
    
    // Transform
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    // Up or no direction found
    if (playerDirection <= 1) {
        entity.assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2);
        entity.assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 5.0f, 5.0f, 1.0, 0.5f, 1); 
    
    // Down
    } else if (playerDirection == 2) {
        entity.assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos - spriteOffset, 0.0f, 0, 0, 0, 1, 2);
        entity.assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos - spriteOffset, 5.0f, 5.0f, 1.0, 0.5f, 1); 
    
    // Right
    } else if (playerDirection == 3) {
        entity.assign<Transform>(playerTransform.get()->xpos + spriteOffset, playerTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 
        entity.assign<RigidBody>(playerTransform.get()->xpos + spriteOffset, playerTransform.get()->ypos, 5.0f, 5.0f, 1.0, 0.5f, 1);
    
    // Left
    } else if (playerDirection == 4) {
        entity.assign<Transform>(playerTransform.get()->xpos - spriteOffset, playerTransform.get()->ypos, 0.0f, 0, 0, 0.0f, 1, 2);
        entity.assign<RigidBody>(playerTransform.get()->xpos - spriteOffset, playerTransform.get()->ypos, 5.0f, 5.0f, 1.0, 0.5f, 1); 
    }

    ComponentHandle<RigidBody> physicsComp = entity.component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->createFixture();

    // Add entity to list
    projectiles.push_back(std::make_pair(ECS::instance().entities.get(entity.id()), playerDirection));
    projectilesTimeElapsed.push_back(0.0f);
}

// Collision detection
void BowScript::beginContact() {}

void BowScript::endContact() {}
