#include "playerScript.h"

#include <string>

#include "../components.h"
#include "../logger.h"
#include "../input.h"
#include "../ECS.h"

PlayerScript::PlayerScript(entityx::Entity* entity) : CScript(entity) {
    canPlayerMove = true;
}

void PlayerScript::start() {
    // Set up collisions
    ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    rigidBody.get()->body->GetUserData().pointer = reinterpret_cast<uintptr_t>(&entity);

    ComponentHandle<Name> entityName;
    for (Entity e : ECS::instance().entities.entities_with_components(entityName)) {
        
        entityName = e.component<Name>();
        if (entityName.get()->name == "PlayerWeapon1")
            weapon1 = e;

        if (entityName.get()->name == "PlayerWeapon2")
            weapon2 = e;
    
    }
}

void PlayerScript::update(TimeDelta dt) {

    // Movement
    float desiredVelX = 0;
    float desiredVelY = 0;

    if (canPlayerMove) {
        // Movement UP
        if (Input::instance().isKeyPressed(GLFW_KEY_W)) {
            desiredVelY = 250;

            ComponentHandle<TextureComp> textureComp = entity.component<TextureComp>();
            textureComp.get()->setTexture("src/Assets/textures/PlayerUp.png");
        }

        // Movement DOWN
        if (Input::instance().isKeyPressed(GLFW_KEY_S)) {
            desiredVelY = -250;

            ComponentHandle<TextureComp> textureComp = entity.component<TextureComp>();
            textureComp.get()->setTexture("src/Assets/textures/PlayerDown.png");
        }
        
        // Movement RIGHT
        if (Input::instance().isKeyPressed(GLFW_KEY_D)) {
            desiredVelX = 250;

            ComponentHandle<TextureComp> textureComp = entity.component<TextureComp>();
            textureComp.get()->setTexture("src/Assets/textures/PlayerRight.png");
        }
        
        // Movement LEFT
        if (Input::instance().isKeyPressed(GLFW_KEY_A)) {
            desiredVelX = -250;

            ComponentHandle<TextureComp> textureComp = entity.component<TextureComp>();
            textureComp.get()->setTexture("src/Assets/textures/PlayerLeft.png");
        }    
    }
    
    // Apply forces
    ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    b2Vec2 playerVelocity = rigidBody.get()->body->GetLinearVelocity();
    
    float velChangeX = desiredVelX - playerVelocity.x;
    float velChangeY = desiredVelY - playerVelocity.y;
    float impulseX = rigidBody.get()->body->GetMass() * velChangeX;
    float impulseY = rigidBody.get()->body->GetMass() * velChangeY;
    rigidBody.get()->body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), rigidBody.get()->body->GetWorldCenter(), true);

    // Update player position
    ComponentHandle<Transform> transform = entity.component<Transform>();
    transform.get()->xpos = rigidBody.get()->body->GetPosition().x;
    transform.get()->ypos = rigidBody.get()->body->GetPosition().y;
    
    
    // Attack
    // Use weapon 1
    if (Input::instance().isKeyPressed(GLFW_KEY_K)) {
        ComponentHandle<Script> scriptComp = weapon1.component<Script>();
        WeaponScript* weaponScript = reinterpret_cast<WeaponScript*>(scriptComp.get()->script);
        if (weaponScript)
            weaponScript->useWeapon();
    } else {
        ComponentHandle<Script> scriptComp = weapon1.component<Script>();
        WeaponScript* weaponScript = reinterpret_cast<WeaponScript*>(scriptComp.get()->script);
        if (weaponScript) 
            weaponScript->setIsActive(false);
    }

    // Use weapon 2
    if (Input::instance().isKeyPressed(GLFW_KEY_L)) {
        ComponentHandle<Script> scriptComp = weapon2.component<Script>();
        WeaponScript* weaponScript = reinterpret_cast<WeaponScript*>(scriptComp.get()->script);
        if (weaponScript)
            weaponScript->useWeapon();
    } else {
        ComponentHandle<Script> scriptComp = weapon2.component<Script>();
        WeaponScript* weaponScript = reinterpret_cast<WeaponScript*>(scriptComp.get()->script);
        if (weaponScript) 
            weaponScript->setIsActive(false);
    }

}

void PlayerScript::beginContact(Entity* entityA, Entity* entityB) {
    ComponentHandle<Name> entityName = entityB->component<Name>();
    if (entityName.get()->name.find("Enemy") != std::string::npos)
        LOG_INFO("Player script made contact with something");
}

void PlayerScript::endContact(Entity* entityA, Entity* entityB) {

}
