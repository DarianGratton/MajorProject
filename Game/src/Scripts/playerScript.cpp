#include "playerScript.h"

#include <string>

#include "../playerPrefs.h"
#include "../scriptFactory.h"
#include "../components.h"
#include "../logger.h"
#include "../input.h"
#include "../ECS.h"

PlayerScript::PlayerScript(entityx::Entity* entity) : CScript(entity) {
    // Initialize variables
    health = 100;
    canPlayerMove = true;
}

void PlayerScript::start() {
    ComponentHandle<Name> entityName;
    for (Entity e : ECS::instance().entities.entities_with_components(entityName)) {
        
        entityName = e.component<Name>();
        if (entityName.get()->name == "PlayerWeapon1")
            weapon1 = e;

        if (entityName.get()->name == "PlayerWeapon2")
            weapon2 = e;
    
    }

    // Note: should be calling start but for some reason it does it for me, something weird with how entityx does for loops.
    if (weapon1.valid() && !weapon1.has_component<Script>()) {
        std::string scriptName = getScriptName(PlayerPrefs::instance().getWeapon1());
        weapon1.assign<Script>(scriptName, &weapon1);
    }

    if (weapon2.valid() && !weapon2.has_component<Script>()) {
        std::string scriptName = getScriptName(PlayerPrefs::instance().getWeapon2());
        weapon2.assign<Script>(scriptName, &weapon2);
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
    if (weapon1.valid()) {
        
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
   
    }

    // Use weapon 2
    if (weapon2.valid()) {
       
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

}

std::string PlayerScript::getScriptName(int i) {
    switch(i) {
        case 1:
            return "SwordScript";
        case 2:
            return "ShieldScript";
        case 3:
            return "BowScript";
        case 4:
            return "GrenadeScript";
        case 5:
            return "GauntletScript";
        default:
            return "SwordScript";
    }
}

void PlayerScript::beginContact(Entity* entityA, Entity* entityB) {
    ComponentHandle<Name> entityName = entityB->component<Name>();
    if (entityName.get()->name.find("Weapon") != std::string::npos) {

        ComponentHandle<Script> weaponScript = entityB->component<Script>();
        int damage = reinterpret_cast<WeaponScript*>(weaponScript.get()->script)->getDamage();
        health -= damage;

    }
}

void PlayerScript::endContact(Entity* entityA, Entity* entityB) {

}
