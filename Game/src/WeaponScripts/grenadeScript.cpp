#include "grenadeScript.h"

#include "../Physics/physicsManager.h"
#include "../components.h"
#include "../scripts.h"
#include "../ECS.h"
#include "../cScript.h"

// Blocks attack
// Cooldown between blocks
// Small knockback after block

GrenadeScript::GrenadeScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity), spriteHeight(spriteHeight), spriteWidth(spriteWidth) {
    spriteOffset = 10.0f;
    projectileSpeed = 0.7f;
    projectileLifespan = 1.5f;
    explosionLifespan = 3.0f;
    cooldownTimer = 0.0f;
    isActive = false;
    timeElapsed = 0.0f;
}

void GrenadeScript::start() {
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = e.component<Name>();
        
        if (entityName.get()->name == "Player")
            player = e;
    }
}

void GrenadeScript::update(TimeDelta dt) {

    // Update cooldown
    float cooldown = cooldownTimer - dt;
    if (cooldown <= 0) {
        cooldownTimer = 0;
    } else {
        cooldownTimer = cooldown;
    }
    timeElapsed += dt;

    // Explode grenade
    if (grenadeEntity.valid() && timeElapsed >= projectileLifespan) {
        spawnExplosion();

        ComponentHandle<RigidBody> physicsComp = grenadeEntity.component<RigidBody>();
        PhysicsManager::instance().getWorld()->DestroyBody(physicsComp.get()->body);

        grenadeEntity.destroy();
        directionThrown = 0;
        timeElapsed = 0.0f;
    }

    // Destroy explosion
    if (explosionEntity.valid() && timeElapsed >= explosionLifespan) {
        ComponentHandle<RigidBody> physicsComp = explosionEntity.component<RigidBody>();
        PhysicsManager::instance().getWorld()->DestroyBody(physicsComp.get()->body);

        explosionEntity.destroy();
        directionThrown = 0;
    }

    // Update position of arrows
    if (grenadeEntity.valid()) { 
        ComponentHandle<Transform> entityTransform = grenadeEntity.component<Transform>();

        // Up or no direction found
        if (directionThrown <= 1) {
            entityTransform.get()->ypos += projectileSpeed;
        
        // Down
        } else if (directionThrown == 2) {
            entityTransform.get()->ypos -= projectileSpeed;  
        
        // Right
        } else if (directionThrown == 3) {
            entityTransform.get()->xpos += projectileSpeed; 
        
        // Left
        } else if (directionThrown == 4) {
            entityTransform.get()->xpos -= projectileSpeed; 
        }

        ComponentHandle<RigidBody> entityBody = grenadeEntity.component<RigidBody>();
        entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0);
    }
}

void GrenadeScript::useWeapon() {
    if (cooldownTimer > 0 || isActive)
        return;
        
    cooldownTimer = projectileLifespan + explosionLifespan + 3.0f;
    timeElapsed = 0.0f;
    spawnGrenade();
}

// Up = 1, Down = 2, Right = 3, Left = 4
int GrenadeScript::getPlayerDirection() {
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

void GrenadeScript::spawnGrenade() {
    // Set up entity components
    grenadeEntity = ECS::instance().entities.create();

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
    
    // TODO: Factor for enemy using weapon
    // Rigidbody bits
    uint16 categoryBit = PhysicsManager::instance().PLAYERWEAPON;
    uint16 maskBit = PhysicsManager::instance().BOUNDARY | PhysicsManager::instance().ENEMY;

    // Transform
    int playerDirection = getPlayerDirection();
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    // Up or no direction found
    if (playerDirection <= 1) {
        grenadeEntity.assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2);
    
    // Down
    } else if (playerDirection == 2) {
        grenadeEntity.assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos - spriteOffset, 0.0f, 0, 0, 0, 1, 2);
    
    // Right
    } else if (playerDirection == 3) {
        grenadeEntity.assign<Transform>(playerTransform.get()->xpos + spriteOffset, playerTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 
    
    // Left
    } else if (playerDirection == 4) {
        grenadeEntity.assign<Transform>(playerTransform.get()->xpos - spriteOffset, playerTransform.get()->ypos, 0.0f, 0, 0, 0.0f, 1, 2); 
    }
    grenadeEntity.assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 1.0f, 1.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 

    ComponentHandle<RigidBody> physicsComp = grenadeEntity.component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->createFixture();
    physicsComp.get()->setUserData(&grenadeEntity);

    // Update variables
    directionThrown = playerDirection;
}

void GrenadeScript::spawnExplosion() {

    // Set up entity components
    explosionEntity = ECS::instance().entities.create();

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

    explosionEntity.assign<Name>("Explosion");
    
    // TODO: Factor for enemy using weapon
    // Rigidbody bits
    uint16 categoryBit = PhysicsManager::instance().PLAYERWEAPON;
    uint16 maskBit = PhysicsManager::instance().BOUNDARY | PhysicsManager::instance().ENEMY | PhysicsManager::instance().PLAYER;

    // Transform
    int playerDirection = getPlayerDirection();
    ComponentHandle<Transform> grenadeTransform = grenadeEntity.component<Transform>(); 
    explosionEntity.assign<Transform>(grenadeTransform.get()->xpos, grenadeTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2);
    explosionEntity.assign<RigidBody>(grenadeTransform.get()->xpos, grenadeTransform.get()->ypos, 10.0f, 10.0f, 1.0, 0.5f, 1, categoryBit, maskBit); 

    explosionEntity.assign<Script>(cscript);

    ComponentHandle<RigidBody> physicsComp = explosionEntity.component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->createFixture();
    physicsComp.get()->setUserData(&explosionEntity);

}

// Collision detection
void GrenadeScript::beginContact(Entity* entityA, Entity* entityB) {
    
}

void GrenadeScript::endContact(Entity* entityA, Entity* entityB) {

}
