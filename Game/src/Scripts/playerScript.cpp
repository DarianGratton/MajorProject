#include "playerScript.h"

#include "../components.h"
#include "../logger.h"
#include "../input.h"

PlayerScript::PlayerScript(entityx::Entity* entity) : CScript(entity) {}

void PlayerScript::start() {

}

void PlayerScript::update() {

    // Movement UP
    if (Input::instance().isKeyPressed(GLFW_KEY_W)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        transform.get()->ypos += 1.0f;
    }

    // Movement DOWN
    if (Input::instance().isKeyPressed(GLFW_KEY_S)) {
        // ComponentHandle<Transform> transform = entity.component<Transform>();
        // transform.get()->ypos -= 1.0f;

        ComponentHandle<RigidBody> rigidBody = entity.component<RigidBody>();
        b2Vec2 playerVelocity = rigidBody.get()->body->GetLinearVelocity();
        float desiredVel = -5;
        float velChange = desiredVel - playerVelocity.x;
        float impluse = rigidBody.get()->body->GetMass() * velChange;
        rigidBody.get()->body->ApplyLinearImpulse(b2Vec2(0, impluse), rigidBody.get()->body->GetWorldCenter(), true);
    }
    
    // Movement RIGHT
    if (Input::instance().isKeyPressed(GLFW_KEY_D)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        transform.get()->xpos += 1.0f;
    }
    
    // Movement LEFT
    if (Input::instance().isKeyPressed(GLFW_KEY_A)) {
        ComponentHandle<Transform> transform = entity.component<Transform>();
        transform.get()->xpos -= 1.0f;
    }    
}