#include "playerScript.h"

#include "../components.h"
#include "../logger.h"
#include "../input.h"

PlayerScript::PlayerScript(entityx::Entity* entity) : CScript(entity) {}

void PlayerScript::start() {

}

void PlayerScript::update() {

    float desiredVelX = 0;
    float desiredVelY = 0;

    // Movement UP
    if (Input::instance().isKeyPressed(GLFW_KEY_W)) {
        // ComponentHandle<Transform> transform = entity.component<Transform>();
        // transform.get()->ypos += 1.0f;
        desiredVelY = 250;
    }

    // Movement DOWN
    if (Input::instance().isKeyPressed(GLFW_KEY_S)) {
        desiredVelY = -250;
    }
    
    // Movement RIGHT
    if (Input::instance().isKeyPressed(GLFW_KEY_D)) {
        desiredVelX = 250;
    }
    
    // Movement LEFT
    if (Input::instance().isKeyPressed(GLFW_KEY_A)) {
        desiredVelX = -250;
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