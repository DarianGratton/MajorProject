#include "shieldScript.h"

#include "../Physics/physicsManager.h"
#include "../components.h"
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
    getEntity()->assign<Transform>(playerTransform.get()->xpos, playerTransform.get()->ypos + 7.0f, 0.0f, 0, 0, 0, 1, 2); 

    // RigidBody
    getEntity()->assign<RigidBody>(playerTransform.get()->xpos, playerTransform.get()->ypos + 7.0f, 5.0f, 2.0f);

    ComponentHandle<RigidBody> physicsComp = getEntity()->component<RigidBody>();
    physicsComp.get()->body = PhysicsManager::instance().getWorld()->CreateBody(&physicsComp.get()->bodyDef);
    physicsComp.get()->createFixture();
}

void ShieldScript::update() {
    if (isActive) {
        if (cooldown > 0)
            cooldown--;
        else
            isActive = false;

        // Update transform
        ComponentHandle<Transform> entityTransform = getEntity()->component<Transform>();
        ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
        entityTransform.get()->xpos = playerTransform.get()->xpos;
        entityTransform.get()->ypos = playerTransform.get()->ypos + 7.0f;

        // Update body
        ComponentHandle<RigidBody> entityBody = getEntity()->component<RigidBody>();
        entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0.0f);
    }
}

void ShieldScript::useWeapon() {
    if (isActive)
        return;

    isActive = true; 
    cooldown = 10;

    // Update transform
    ComponentHandle<Transform> entityTransform = getEntity()->component<Transform>();
    ComponentHandle<Transform> playerTransform = player.component<Transform>(); 
    entityTransform.get()->xpos = playerTransform.get()->xpos;
    entityTransform.get()->ypos = playerTransform.get()->ypos + 6.0f;

    // Update body
    ComponentHandle<RigidBody> entityBody = getEntity()->component<RigidBody>();
    entityBody.get()->body->SetTransform(b2Vec2(entityTransform.get()->xpos, entityTransform.get()->ypos), 0.0f);
}

// Collision detection
void ShieldScript::beginContact() {

}

void ShieldScript::endContact() {

}
