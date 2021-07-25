#include "swordScript.h"

#include "../Physics/physicsManager.h"
#include "../components.h"
#include "../scripts.h"
#include "../ECS.h"

// Blocks attack
// Cooldown between blocks
// Small knockback after block

SwordScript::SwordScript(Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity) {
    spriteOffset = 10.0f;
    cooldown = 0;
    isActive = false;
}

void SwordScript::start() {
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "Player")
            player = entity;
    }

    // Set up entity components
    getEntity()->assign<TextureComp>("src/Assets/textures/SwordSlash.png", "SwordSlash.png");
    getEntity()->assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    
    // Transform
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    getEntity()->assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 0.0f, 0, 0, 0, 1, 2); 

    // RigidBody
    getEntity()->assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos + spriteOffset, 5.0f, 5.0f, 1.0, 0.5f, 0);

    ComponentHandle<RigidBody> physicsComp = getEntity()->component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->createFixture();

    // Active
    ComponentHandle<Active> activeComp = getEntity()->component<Active>();
    activeComp.get()->isActive = false;
    physicsComp.get()->body->SetEnabled(false);
}

void SwordScript::update(TimeDelta dt) {
    ComponentHandle<RigidBody> entityBody = getEntity()->component<RigidBody>();

    if (entityBody.get()->body->IsEnabled()) {

        if (cooldown > 0) {
            cooldown--;
            return;
        }

        // Disable collision
        entityBody.get()->body->SetEnabled(false);

        // Disable entity
        ComponentHandle<Active> activeComp = getEntity()->component<Active>();
        activeComp.get()->isActive = false;

        // Enable player movement
        ComponentHandle<Script> playerScript = player.component<Script>(); 
        reinterpret_cast<PlayerScript*>(playerScript.get()->script)->setCanPlayerMove(true);
    }
}

void SwordScript::useWeapon() {
    if (isActive)
        return;

    isActive = true;
    cooldown = 3;

    // Update sword's transform
    ComponentHandle<Transform> entityTransform = getEntity()->component<Transform>();
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 

    int playerDirection = getPlayerDirection();
    // Up or no direction found
    if (playerDirection <= 1) {
        entityTransform.get()->xpos = playerTransform.get()->xpos;
        entityTransform.get()->ypos = playerTransform.get()->ypos + spriteOffset; 
    
    // Down
    } else if (playerDirection == 2) {
        entityTransform.get()->xpos = playerTransform.get()->xpos;
        entityTransform.get()->ypos = playerTransform.get()->ypos - spriteOffset; 
    
    // Right
    } else if (playerDirection == 3) {
        entityTransform.get()->xpos = playerTransform.get()->xpos + spriteOffset;
        entityTransform.get()->ypos = playerTransform.get()->ypos; 
    
    // Left
    } else if (playerDirection == 4) {
        entityTransform.get()->xpos = playerTransform.get()->xpos - spriteOffset;
        entityTransform.get()->ypos = playerTransform.get()->ypos; 
    }

    // Update body transform
    ComponentHandle<RigidBody> entityBody = getEntity()->component<RigidBody>();
    entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0.0f);

    // Disable player movement
    ComponentHandle<Script> playerScript = player.component<Script>(); 
    reinterpret_cast<PlayerScript*>(playerScript.get()->script)->setCanPlayerMove(false);

    // Activate entity
    ComponentHandle<Active> activeComp = getEntity()->component<Active>();
    activeComp.get()->isActive = true;
    entityBody.get()->body->SetEnabled(true);
}

// Up = 1, Down = 2, Right = 3, Left = 4
int SwordScript::getPlayerDirection() {
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

// Collision detection
void SwordScript::beginContact() {

}

void SwordScript::endContact() {

}
