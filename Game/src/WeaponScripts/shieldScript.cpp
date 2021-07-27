#include "shieldScript.h"

#include "../Physics/physicsManager.h"
#include "../components.h"
#include "../scripts.h"
#include "../ECS.h"

// Blocks attack
// Cooldown between blocks
// Small knockback after block

ShieldScript::ShieldScript(entityx::Entity* entity, float spriteHeight, float spriteWidth) : WeaponScript(entity), spriteHeight(spriteHeight), spriteWidth(spriteWidth) {
    shieldCooldown = 0;
    isActive = false;
    hitVelocity = b2Vec2_zero;
}

void ShieldScript::start() {
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = e.component<Name>();
        
        if (entityName.get()->name == "Player")
            player = e;
    }

    // Set up entity components
    getEntity()->assign<TextureComp>("src/Assets/textures/Shield.png");
    getEntity()->assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    
    // Transform
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    getEntity()->assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 

    getEntity()->remove<SpriteVertices>();
    std::vector<float> spriteVertices =  {
            -spriteWidth/2, -spriteHeight/2, 0.0f, 0.0f,
             spriteWidth/2, -spriteHeight/2, 1.0f, 0.0f,
             spriteWidth/2,  spriteHeight/2, 1.0f, 1.0f,
            -spriteWidth/2,  spriteHeight/2, 0.0f, 1.0f
        };
    getEntity()->assign<SpriteVertices>(spriteVertices);

    // RigidBody
    // TODO: Factor for enemy using weapon
    uint16 categoryBit = PhysicsManager::instance().PLAYERWEAPON;
    uint16 maskBit = PhysicsManager::instance().BOUNDARY | PhysicsManager::instance().ENEMY | PhysicsManager::instance().ENEMYWEAPON;
    getEntity()->assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos, 5.0f, 5.0f, 1.0, 0.5f, 1, categoryBit, maskBit);

    ComponentHandle<RigidBody> physicsComp = getEntity()->component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->createFixture();

    // Active
    ComponentHandle<Active> activeComp = getEntity()->component<Active>();
    activeComp.get()->isActive = false;
    physicsComp.get()->body->SetEnabled(false);
}

void ShieldScript::update(TimeDelta dt) {

    // Update cooldown
    float cooldown = shieldCooldown - dt;
    if (cooldown <= 0) {
        shieldCooldown = 0;
    } else {
        shieldCooldown = cooldown;
        return;
    }

    ComponentHandle<RigidBody> entityBody = getEntity()->component<RigidBody>();
    if (!isActive && entityBody.get()->body->IsEnabled()) {
        // Disable collision
        entityBody.get()->body->SetEnabled(false);

        // Disable entity
        ComponentHandle<Active> activeComp = getEntity()->component<Active>();
        activeComp.get()->isActive = false;

        // Enable player movement
        ComponentHandle<Script> playerScript = player.component<Script>(); 
        reinterpret_cast<PlayerScript*>(playerScript.get()->script)->setCanPlayerMove(true);

        // Enable player body
        ComponentHandle<RigidBody> playerBody = player.component<RigidBody>(); 
        playerBody.get()->body->SetEnabled(true);
        playerBody.get()->body->SetLinearVelocity(hitVelocity);
        hitVelocity = b2Vec2_zero;
    }
}

void ShieldScript::useWeapon() {
    if (isActive)
        return;

    isActive = true; 

    // Update shield's transform
    ComponentHandle<Transform> entityTransform = getEntity()->component<Transform>();
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    entityTransform.get()->xpos = playerTransform.get()->xpos;
    entityTransform.get()->ypos = playerTransform.get()->ypos;

    // Update body transform
    ComponentHandle<RigidBody> entityBody = getEntity()->component<RigidBody>();
    entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0.0f);

    // Disable player movement
    ComponentHandle<Script> playerScript = player.component<Script>(); 
    reinterpret_cast<PlayerScript*>(playerScript.get()->script)->setCanPlayerMove(false);

    // Disable player body
    ComponentHandle<RigidBody> playerBody = player.component<RigidBody>(); 
    playerBody.get()->body->SetEnabled(false);

    // Activate entity
    ComponentHandle<Active> activeComp = getEntity()->component<Active>();
    activeComp.get()->isActive = true;
    entityBody.get()->body->SetEnabled(true);
}

// Collision detection
void ShieldScript::beginContact(Entity* entityA, Entity* entityB) {
    ComponentHandle<Name> nameComp = entityB->component<Name>();
    if (!(nameComp.get()->name.find("Enemy") != std::string::npos)) {
        isActive = false;

        ComponentHandle<RigidBody> shieldBody = entityA->component<RigidBody>();
        hitVelocity = shieldBody.get()->body->GetLinearVelocity();

        shieldCooldown = 3.0f;
    }
}

void ShieldScript::endContact(Entity* entityA, Entity* entityB) {

}
