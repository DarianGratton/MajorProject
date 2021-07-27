#include "bowScript.h"

#include "../Physics/physicsManager.h"
#include "../components.h"
#include "../scripts.h"
#include "../ECS.h"
#include "../cScript.h"

// Blocks attack
// Cooldown between blocks
// Small knockback after block

BowScript::BowScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity), spriteHeight(spriteHeight), spriteWidth(spriteWidth) {
    spriteOffset = 10.0f;
    projectileSpeed = 2.0f;
    projectileLifespan = 2.0f;
    fireRate = 0.0f;
    isActive = false;
    arrowNumber = 0;
}

void BowScript::start() {
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = e.component<Name>();
        
        if (entityName.get()->name == "Player")
            player = e;
    }
}

void BowScript::update(TimeDelta dt) {

    // Delete any projectiles flagged for deletion
    for (std::string entityName : projectilesFlaggedForDeletion) {
        for (int i = 0; i < projectilesTimeElapsed.size(); i++) {
            
            ComponentHandle<Name> nameComp = projectiles.at(i).first.component<Name>();
            if (nameComp.get()->name == entityName) {
                Entity e = projectiles.at(i).first;
                projectiles.erase(projectiles.begin() + i);
                projectilesTimeElapsed.erase(projectilesTimeElapsed.begin() + i);

                ComponentHandle<RigidBody> physicsComp = e.component<RigidBody>();
                PhysicsManager::instance().getWorld()->DestroyBody(physicsComp.get()->body);

                e.destroy();
                break;
            }
        
        }
    }
    projectilesFlaggedForDeletion.clear();


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

    fireRate = 0.8f;
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
    Entity e = ECS::instance().entities.create();

    int playerDirection = getPlayerDirection();
    if (playerDirection <= 1) {
        e.assign<TextureComp>("src/Assets/textures/ArrowUp.png", "ArrowUp.png");
    } else if (playerDirection == 2) {
        e.assign<TextureComp>("src/Assets/textures/ArrowDown.png", "ArrowDown.png");
    } else if (playerDirection == 3) {
        e.assign<TextureComp>("src/Assets/textures/ArrowRight.png", "ArrowRight.png");
    } else if (playerDirection == 4) {
        e.assign<TextureComp>("src/Assets/textures/ArrowLeft.png", "ArrowLeft.png");
    }

    e.assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    e.assign<Active>(true);

    std::vector<float> spriteVertices =  {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f,
        };
    e.assign<SpriteVertices>(spriteVertices);

    std::string name = "Arrow" + std::to_string(arrowNumber);
    e.assign<Name>(name);
    
    // TODO: Factor for enemy using weapon
    // Rigidbody bits
    uint16 categoryBit = PhysicsManager::instance().PLAYERWEAPON;
    uint16 maskBit = PhysicsManager::instance().BOUNDARY | PhysicsManager::instance().ENEMY | PhysicsManager::instance().ENEMYWEAPON;

    // Transform
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    // Up or no direction found
    if (playerDirection <= 1) {
        e.assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2);
        e.assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 2.0f, 4.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
    
    // Down
    } else if (playerDirection == 2) {
        e.assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos - spriteOffset, 0.0f, 0, 0, 0, 1, 2);
        e.assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos - spriteOffset, 2.0f, 4.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
    
    // Right
    } else if (playerDirection == 3) {
        e.assign<Transform>(playerTransform.get()->xpos + spriteOffset, playerTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 
        e.assign<RigidBody>(playerTransform.get()->xpos + spriteOffset, playerTransform.get()->ypos, 4.0f, 2.0f, 1.0, 0.5f, 1, categoryBit, maskBit);
    
    // Left
    } else if (playerDirection == 4) {
        e.assign<Transform>(playerTransform.get()->xpos - spriteOffset, playerTransform.get()->ypos, 0.0f, 0, 0, 0.0f, 1, 2);
        e.assign<RigidBody>(playerTransform.get()->xpos - spriteOffset, playerTransform.get()->ypos, 4.0f, 2.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 
    }

    e.assign<Script>(cscript);

    ComponentHandle<RigidBody> physicsComp = e.component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->createFixture();
    physicsComp.get()->setUserData(&e);

    // Add entity to list
    projectiles.push_back(std::make_pair(ECS::instance().entities.get(e.id()), playerDirection));
    projectilesTimeElapsed.push_back(0.0f);
    arrowNumber++;
    if (arrowNumber > 2) {
        arrowNumber = 0;
    }
}

// Collision detection
void BowScript::beginContact(Entity* entityA, Entity* entityB) {
    ComponentHandle<Name> nameComp = entityA->component<Name>();
    if (nameComp.get()->name.find("Arrow") != std::string::npos) {
        projectilesFlaggedForDeletion.push_back(nameComp.get()->name);
    }
}

void BowScript::endContact(Entity* entityA, Entity* entityB) {

}
