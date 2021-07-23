#include "playerScript.h"

#include "../components.h"
#include "../logger.h"
#include "../input.h"
#include "../ECS.h"

PlayerScript::PlayerScript(entityx::Entity* entity) : CScript(entity) {}

void PlayerScript::start() {
    // Set up collisions
    ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
    rigidBody.get()->body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    ComponentHandle<Name> entityName;
    for (Entity entity : ECS::instance().entities.entities_with_components(entityName)) {
        entityName = entity.component<Name>();
        
        if (entityName.get()->name == "PlayerWeapon1")
            weapon1 = entity;

        if (entityName.get()->name == "PlayerWeapon2")
            weapon2 = entity;
    }
}

void PlayerScript::update() {

    float desiredVelX = 0;
    float desiredVelY = 0;

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
}

void PlayerScript::beginContact() {
    LOG_INFO("Player script made contact");
}

void PlayerScript::endContact() {

}
