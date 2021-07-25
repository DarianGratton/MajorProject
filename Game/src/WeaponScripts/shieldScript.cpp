#include "shieldScript.h"

#include "../Physics/physicsManager.h"
#include "../components.h"
#include "../scripts.h"
#include "../ECS.h"

// Blocks attack
// Cooldown between blocks
// Small knockback after block

ShieldScript::ShieldScript(entityx::Entity* entity) : WeaponScript(entity) {
    cooldown = 0;
    isActive = false;
}

void ShieldScript::start() {
    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "Player")
            player = entity;
    }

    // Set up entity components
    getEntity()->assign<TextureComp>("src/Assets/textures/Shield.png");
    getEntity()->assign<ShaderComp>("src/Assets/shaders/Basic.shader");
    
    // Transform
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    getEntity()->assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos, 0.0f, 0, 0, 0, 1, 2); 

    // RigidBody
    getEntity()->assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos, 5.0f, 5.0f, 1.0, 0.5f, 1);

    ComponentHandle<RigidBody> physicsComp = getEntity()->component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->createFixture();

    // Active
    ComponentHandle<Active> activeComp = getEntity()->component<Active>();
    activeComp.get()->isActive = false;
    physicsComp.get()->body->SetEnabled(false);
}

void ShieldScript::update(TimeDelta dt) {
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
    }

    if (isActive) {
        // TODO: Might need to replace with a knockback function or something
        // Note: Hard to test but this should make it so if player is hit while shield is out the player follows the knockback of the shield
        // Update shield's transform
        ComponentHandle<Transform> entityTransform = getEntity()->component<Transform>();
        ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
        entityTransform.get()->xpos = entityBody.get()->body->GetPosition().x;
        entityTransform.get()->ypos = entityBody.get()->body->GetPosition().y;
        playerTransform.get()->xpos = entityBody.get()->body->GetPosition().x;
        playerTransform.get()->ypos = entityBody.get()->body->GetPosition().y;
    }
}

void ShieldScript::useWeapon() {
    if (isActive)
        return;

    isActive = true; 
    cooldown = 100;

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
void ShieldScript::beginContact() {

}

void ShieldScript::endContact() {

}
